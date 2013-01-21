/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: AuthServer.cpp
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "AuthServer.h"
#include "AuthCodes.h"
#include "ByteBuffer.h"

#include "..\Util\Common.h"
#include "..\Util\Logs.h"
#include "..\Util\Database\DatabaseEnv.h"

#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_fcntl.h>
#include <ace/OS_NS_sys_stat.h>

extern DatabaseType LoginDatabase;

enum eStatus
{
    STATUS_CONNECTED = 0,
    STATUS_AUTHED
};

enum AccountFlags
{
    ACCOUNT_FLAG_GM         = 0x00000001,
    ACCOUNT_FLAG_TRIAL      = 0x00000008,
    ACCOUNT_FLAG_PROPASS    = 0x00800000,
};

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

typedef struct AUTH_LOGON_CHALLENGE_C
{
    uint8   cmd;
    uint8   error;
    uint16  size;
    uint8   gamename[4];
    uint8   version1;
    uint8   version2;
    uint8   version3;
    uint16  build;
    uint8   platform[4];
    uint8   os[4];
    uint8   country[4];
    uint32  timezone_bias;
    uint32  ip;
    uint8   I_len;
    uint8   I[1];
} sAuthLogonChallenge_C;

typedef struct AUTH_LOGON_PROOF_C
{
    uint8   cmd;
    uint8   A[32];
    uint8   M1[20];
    uint8   crc_hash[20];
    uint8   number_of_keys;
    uint8   securityFlags;                                  // 0x00-0x04
} sAuthLogonProof_C;

typedef struct AUTH_LOGON_PROOF_S
{
    uint8   cmd;
    uint8   error;
    uint8   M2[20];
    uint32  accountFlags;                                   // see enum AccountFlags
    uint32  surveyId;                                       // SurveyId
    uint16  unkFlags;                                       // some flags (AccountMsgAvailable = 0x01)
} sAuthLogonProof_S;


typedef struct AuthHandler
{
    eAuthCmd cmd;
    uint32 status;
    bool (AuthServer::*handler)(void);
} AuthHandler;

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

const AuthHandler table[] =
{
    { CMD_AUTH_LOGON_CHALLENGE,     STATUS_CONNECTED, &AuthServer::_HandleLogonChallenge    },
    { CMD_AUTH_LOGON_PROOF,         STATUS_CONNECTED, &AuthServer::_HandleLogonProof        },
    { CMD_AUTH_RECONNECT_CHALLENGE, STATUS_CONNECTED, &AuthServer::_HandleReconnectChallenge},
    { CMD_AUTH_RECONNECT_PROOF,     STATUS_CONNECTED, &AuthServer::_HandleReconnectProof    },
    { CMD_REALM_LIST,               STATUS_AUTHED,    &AuthServer::_HandleRealmList         }
};

#define AUTH_TOTAL_COMMANDS sizeof(table)/sizeof(AuthHandler)

// Constructor - set the N and g values for SRP6
AuthServer::AuthServer()
{
	N.SetHexStr("894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7");
	g.SetDword(7);
	_authed = false;

	// account Security Level ->

	_build = 0;
    patch_ = ACE_INVALID_HANDLE;
}

AuthServer::~AuthServer()
{
	if(patch_ != ACE_INVALID_HANDLE)
		ACE_OS::close(patch_);
}

/// Accept the connection and set the s random value for SRP6
void AuthServer::OnAccept()
{
	ASLog.DebugLog("Accepting connection from '%s'", get_remote_address().c_str());
}

// Read the packet from the client
void AuthServer::OnRead()
{
    uint8 _cmd;
    while (1)
    {
        if(!recv_soft((char *)&_cmd, 1))
            return;

        size_t i;

        //- Circle through known commands and call the correct command handler
        for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
        {
            if ((uint8)table[i].cmd == _cmd && (table[i].status == STATUS_CONNECTED || (_authed && table[i].status == STATUS_AUTHED)))
            {
                ASLog.DebugLog("[Auth] got data for cmd %u recv length %u",(uint32)_cmd, (uint32)recv_len());

                if (!(*this.*table[i].handler)())
                {
                    ASLog.DebugLog("Command handler failed for cmd %u recv length %u",(uint32)_cmd, (uint32)recv_len());
                    return;
                }
                break;
            }
        }

        //- Report unknown commands in the debug log
        if (i == AUTH_TOTAL_COMMANDS)
        {
            ASLog.DebugLog("[Auth] got unknown packet %u", (uint32)_cmd);
            return;
        }
    }
}

// Make the SRP6 calculation from hash in dB
void AuthServer::_SetVSFields(const std::string& rI)
{
	s.SetRand(s_BYTE_SIZE * 8);

	BigNumber I;
	I.SetHexStr(rI.c_str());

	// In case of leading zeros in the rI hash, restore them
	uint8 mDigest[SHA_DIGEST_LENGTH];
	memset(mDigest, 0, SHA_DIGEST_LENGTH);
	if (I.GetNumBytes() <= SHA_DIGEST_LENGTH)
	{
		memcpy(mDigest, I.AsByteArray(), I.GetNumBytes());
	}
	std::reverse(mDigest, mDigest + SHA_DIGEST_LENGTH);

	Sha1Hash sha;
	sha.UpdateData(s.AsByteArray(), s.GetNumBytes());
	sha.UpdateData(mDigest, SHA_DIGEST_LENGTH);
	sha.Finalize();
	
	BigNumber x;
	x.SetBinary(sha.GetDigest(), sha.GetLength());
	v = g.ModExp(x, N);
	// No SQL injection (username escaped)
	const char *v_hex, *s_hex;
	v_hex = v.AsHexStr();
	s_hex = s.AsHexStr();
	// update sql ->
	LoginDatabase.PExecute("UPDATE account SET v = '%s', s = '%s' WHERE username = '%s'", v_hex, s_hex, _safelogin.c_str());
	OPENSSL_free((void*)v_hex);
	OPENSSL_free((void*)s_hex);
}

void AuthServer::SendProof(Sha1Hash sha)
{
    switch(_build)
    {
	case 5875:		// 1.12.1
	case 6005:		// 1.12.2
	case 7799:		// 2.3.3 DotaWoW Client
	case 8606:		// 2.4.3
	case 10505:		// 3.2.2a
	case 11159:		// 3.3.0a
	case 11403:		// 3.3.2
	case 11723:		// 3.3.3a
	case 12340:		// 3.3.5a
	default:		// or later
		{
            sAuthLogonProof_S proof;
            memcpy(proof.M2, sha.GetDigest(), 20);
            proof.cmd = CMD_AUTH_LOGON_PROOF;
            proof.error = 0;
            proof.accountFlags = ACCOUNT_FLAG_PROPASS;
            proof.surveyId = 0x00000000;
            proof.unkFlags = 0x0000;

            send((char *)&proof, sizeof(proof));
            break;
        }
    }
}

bool AuthServer::_HandleLogonChallenge()
{
	ASLog.DebugLog("Entering _HandleLogonChallenge");

	if (recv_len() < sizeof(sAuthLogonChallenge_C))
		return false;

    //- Read the first 4 bytes (header) to get the length of the remaining of the packet
    std::vector<uint8> buf;
    buf.resize(4);

    recv((char *)&buf[0], 4);

    EndianConvert(*((uint16*)(buf[0])));
    uint16 remaining = ((sAuthLogonChallenge_C *)&buf[0])->size;
	ASLog.DebugLog("[AuthChallenge] got header, body is %#04x bytes", remaining);

	if ((remaining < sizeof(sAuthLogonChallenge_C) - buf.size()) || (recv_len() < remaining))
		return false;

	// No big fear of memory outage (size is int16, i.e. < 65536)
	buf.resize(remaining + buf.size() + 1);
	buf[buf.size() - 1] = 0;
	sAuthLogonChallenge_C *ch = (sAuthLogonChallenge_C*)&buf[0];

    //- Read the remaining of the packet
    recv((char *)&buf[4], remaining);
	ASLog.DebugLog("[AuthChallenge] got full packet, %#04x bytes", ch->size);
	ASLog.DebugLog("[AuthChallenge] name(%d): %s",ch->I_len, ch->I);

	// BigEndian code, nop in little endian case
	// size already converted
    EndianConvert(*((uint32*)(&ch->gamename[0])));
    EndianConvert(ch->build);
    EndianConvert(*((uint32*)(&ch->platform[0])));
    EndianConvert(*((uint32*)(&ch->os[0])));
    EndianConvert(*((uint32*)(&ch->country[0])));
    EndianConvert(ch->timezone_bias);
    EndianConvert(ch->ip);

    _login = (const char*)ch->I;
    _build = ch->build;
    _os = (const char*)ch->os;

	_safelogin = _login;
	LoginDatabase.escape_string(_safelogin);

	DBRow result = LoginDatabase.PQuery("SELECT sha_pass_hash,id,locked,last_ip,gmlevel,v,s FROM account WHERE username = '%s'",_safelogin.c_str());
	ByteBuffer package;
    package << (uint8) CMD_AUTH_LOGON_CHALLENGE;
    package << (uint8) 0x00;

	// later will be checking
	std::string rI = result[0];

	//- Don't calculate (v, s) if there are already some in the database
	std::string databaseV = result[5];
	std::string databaseS = result[6];

	ASLog.DebugLog("database authentication values: v='%s' s='%s'", databaseV.c_str(), databaseS.c_str());

	// multiply with 2, bytes are stored as hexstring
	if (databaseV.size() != s_BYTE_SIZE*2 || databaseS.size() != s_BYTE_SIZE*2)
	{
		_SetVSFields(rI);
	}
	else
	{
		s.SetHexStr(databaseS.c_str());
		v.SetHexStr(databaseV.c_str());
	}

	b.SetRand(19 * 8);
	BigNumber gmod = g.ModExp(b, N);
	B = ((v * 3) + gmod) % N;

	BigNumber unk3;
	unk3.SetRand(16 * 8);

	///- Fill the response packet with the result
	package << uint8(WOW_SUCCESS);

	// B may be calculated < 32B so we force minimal length to 32B
	package.append(B.AsByteArray(32), 32);      // 32 bytes
	package << uint8(1);
	package.append(g.AsByteArray(), 1);
	package << uint8(32);
	package.append(N.AsByteArray(32), 32);
	package.append(s.AsByteArray(), s.GetNumBytes());// 32 bytes
	package.append(unk3.AsByteArray(16), 16);
	uint8 securityFlags = 0;
	package << uint8(securityFlags);            // security flags (0x0...0x04)


	send((char const*)package.contents(), package.size());

	return true;
}

bool AuthServer::_HandleLogonProof()
{
	ASLog.DebugLog("Entering _HandleLogonProof");
	// Read the packet
    sAuthLogonProof_C lp;
	if (!recv((char *)&lp, sizeof(sAuthLogonProof_C)))
		return false;
	// check client ->

	//- Continue the SRP6 calculation based on data received from the client
	BigNumber A;
	A.SetBinary(lp.A, 32);

	// SRP safeguard: abort if A==0
	if (A.isZero())
		return false;

	//u = H(A, B)
	Sha1Hash sha;
	sha.UpdateBigNumbers(&A, &B, NULL);
	sha.Finalize();
	BigNumber u;
	u.SetBinary(sha.GetDigest(), 20);
	//S = ((A*(v^u % N)) ^ B) % N
	BigNumber S = (A * (v.ModExp(u, N))).ModExp(b, N);

	//M = H( H(N) XOR H(g), H(I), s, A, B, K)
	uint8 t[32];
	uint8 t1[16];
	uint8 vK[40];
    memcpy(t, S.AsByteArray(32), 32);
	for (int i = 0; i < 16; ++i)
	{
		t1[i] = t[i * 2];
	}
	sha.Initialize();
	sha.UpdateData(t1, 16);
	sha.Finalize();
	for (int i = 0; i < 20; ++i)
    {
		vK[i * 2] = sha.GetDigest()[i];
	}
	for (int i = 0; i < 16; ++i)
	{
		t1[i] = t[i * 2 + 1];
	}
	sha.Initialize();
	sha.UpdateData(t1, 16);
	sha.Finalize();
	for (int i = 0; i < 20; ++i)
	{
		vK[i * 2 + 1] = sha.GetDigest()[i];
	}
	K.SetBinary(vK, 40);

	uint8 hash[20];

	sha.Initialize();
	sha.UpdateBigNumbers(&N, NULL);
	sha.Finalize();
	memcpy(hash, sha.GetDigest(), 20);
	sha.Initialize();
	sha.UpdateBigNumbers(&g, NULL);
	sha.Finalize();
	for (int i = 0; i < 20; ++i)
	{
		hash[i] ^= sha.GetDigest()[i];
	}
	BigNumber t3;
	t3.SetBinary(hash, 20);

	sha.Initialize();
	sha.UpdateData(_login);
	sha.Finalize();
	uint8 t4[SHA_DIGEST_LENGTH];
	memcpy(t4, sha.GetDigest(), SHA_DIGEST_LENGTH);

	sha.Initialize();
	sha.UpdateBigNumbers(&t3, NULL);
	sha.UpdateData(t4, SHA_DIGEST_LENGTH);
	sha.UpdateBigNumbers(&s, &A, &B, &K, NULL);
	sha.Finalize();
	BigNumber M;
	M.SetBinary(sha.GetDigest(), 20);

	//- Check if SRP6 results match (password is correct), else send an error
	if (!memcmp(M.AsByteArray(), lp.M1, 20))
	{
		ASLog.MainLog("User '%s' successfully authenticated", _login.c_str());

		//- Update the sessionkey, last_ip, last login time and reset number of failed logins in the account table for this account
		// No SQL injection (escaped user name) and IP address as received by socket
		const char* K_hex = K.AsHexStr();
		// sql update session
		OPENSSL_free((void*)K_hex);

        ///- Finish SRP6 and send the final result to the client
		sha.Initialize();
		sha.UpdateBigNumbers(&A, &M, &K, NULL);
		sha.Finalize();

		SendProof(sha);

		// - Set _authed to true!
		_authed = true;
	}
	else
	{
		char data[4] = { CMD_AUTH_LOGON_PROOF, WOW_FAIL_UNKNOWN_ACCOUNT, 3, 0};
		send(data, sizeof(data));
		ASLog.MainLog("[AuthChallenge] account %s tried to login with wrong password!",_login.c_str ());
	}
	return true;
}

bool AuthServer::_HandleReconnectChallenge()
{
	ASLog.DebugLog("Entering _HandleReconnectChallenge");
	return 0;
}

bool AuthServer::_HandleReconnectProof()
{
	ASLog.DebugLog("Entering _HandleReconnectProof");
	return 0;
}

bool AuthServer::_HandleRealmList()
{
	ASLog.DebugLog("Entering _HandleRealmList");
	return 0;
}