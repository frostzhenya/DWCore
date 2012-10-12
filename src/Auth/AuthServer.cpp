/*
*
*/

#include "AuthServer.h"
#include "AuthCodes.h"
#include <new>

enum AccountFlags
{
    ACCOUNT_FLAG_GM         = 0x00000001,
    ACCOUNT_FLAG_TRIAL      = 0x00000008,
    ACCOUNT_FLAG_PROPASS    = 0x00800000,
};

//typedef sAuthLogonChallenge_C sAuthReconnectChallenge_C;

/*typedef struct
{
    unsigned char   cmd;
    unsigned char   error;
    unsigned char   unk2;
    unsigned char   B[32];
    unsigned char   g_len;
    unsigned char   g[1];
    unsigned char   N_len;
    unsigned char   N[32];
    unsigned char   s[32];
    unsigned char   unk3[16];
} sAuthLogonChallenge_S;
*/

typedef struct AUTH_LOGON_CHALLENGE_C
{
	unsigned char cmd;
	unsigned char error;
	unsigned short size;
	unsigned char gamename[4];
	unsigned char version1;
	unsigned char version2;
	unsigned char version3;
	unsigned short build;
	unsigned char platform[4];
	unsigned char os[4];
	unsigned char country[4];
	unsigned int timezone_bias;
	unsigned int ip;
	unsigned char I_len;
	unsigned char I[1];
} sAuthLogonChallenge_C;

typedef struct AUTH_LOGON_PROOF_C
{
	unsigned char cmd;
	unsigned char A[32];
	unsigned char M1[20];
	unsigned char crc_hash[20];
	unsigned char number_of_keys;
	unsigned char securityFlags; // 0x00-0x04
} sAuthLogonProof_C;

typedef struct AUTH_LOGON_PROOF_S
{
	unsigned char cmd;
	unsigned char error;
	unsigned char M2[20];
	unsigned int accountFlags;// see enum AccountFlags
	unsigned int surveyId;    // SurveyId
	unsigned short unkFlags;  // some flags (AccountMsgAvailable = 0x01)
} sAuthLogonProof_S;

// Constructor - set the N and g values for SRP6
AuthServer::AuthServer()
{
	N.SetHexStr("894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7");
	g.SetDword(7);
	_authed = false;

   // account Security Level ->

    _build = 0;
}

AuthServer::~AuthServer()
{
}

// Make the SRP6 calculation from hash in dB
void AuthServer::_SetVSFields(const std::string& rI)
{
    s.SetRand(s_BYTE_SIZE * 8);

    BigNumber I;
    I.SetHexStr(rI.c_str());

    // In case of leading zeros in the rI hash, restore them
    unsigned char mDigest[SHA_DIGEST_LENGTH];
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
        //case 8606:		// 2.4.3
        //case 10505:		// 3.2.2a
        //case 11159:		// 3.3.0a
        //case 11403:		// 3.3.2
        //case 11723:		// 3.3.3a
        //case 12340:		// 3.3.5a
        default:		// or later
        {
            sAuthLogonProof_S proof;
            memcpy(proof.M2, sha.GetDigest(), 20);
            proof.cmd = CMD_AUTH_LOGON_PROOF;
            proof.error = 0;
            proof.accountFlags = ACCOUNT_FLAG_PROPASS;
            proof.surveyId = 0x00000000;
            proof.unkFlags = 0x0000;

			/*
			* need to handle the structure of proof in BufferedSocket
			*/
            //send((char *)&proof, sizeof(proof)); LogonProof
            break;
        }
    }
}

// Logon Challenge command handler
bool AuthServer::_HandleLogonChallenge()
{
	ASLog.DebugLog("Entering _HandleLogonChallenge\n");

	buf.BuildSocketsAuth();
	buf.BufferAuth_recv(0x1000);

	unsigned short remaining = ((sAuthLogonChallenge_C *)&buf.ByteBuffer)->size;
	ASLog.DebugLog("[AuthChallenge] got header, body is %us#x bytes\n", remaining);

	// dynamic data
	char* dynamicBuff = 0;
	dynamicBuff = new char[buf.SizePackage()];
	for (int i = 0; i < buf.SizePackage(); i++)
	{
		dynamicBuff[i] = buf.ByteBuffer[i];
	}

	sAuthLogonChallenge_C *ch = (sAuthLogonChallenge_C*)dynamicBuff;
	ASLog.DebugLog("[AuthChallenge] got full packet, %us#x bytes\n", ch->size);
	ASLog.DebugLog("[AuthChallenge] name(%d): '%s'\n",ch->I_len, ch->I);

	//_build = ch->build;
	_build = buf.ByteBuffer[11] + 256*buf.ByteBuffer[12]; // fixed
	//_login = (const char*)ch->I;
	//fixed
	for (int i = 1; i <= buf.ByteBuffer[33]; i++)
	{
		_login += buf.ByteBuffer[34+i-1];
	}
    _os = (const char*)ch->os;

	// clear memory
	delete [] dynamicBuff;
	dynamicBuff = 0;

	buf.BufferClear(buf.SizePackage());
	// generate Package
	buf.ByteBuffer[0] = CMD_AUTH_LOGON_CHALLENGE; //fixed in index
	buf.ByteBuffer[1] = 0x00; //fixed in index

	// later will be checking // fixed
	std::string rI;
	// fixed rI
	rI = _login + ":" + _login;
	_SetVSFields(rI);
	
	b.SetRand(19 * 8);
	BigNumber gmod = g.ModExp(b, N);
	B = ((v * 3) + gmod) % N;
	
	BigNumber unk3;
	unk3.SetRand(16 * 8);

	//copy byte
	buf.ByteBuffer[2] = WOW_SUCCESS;
	buf.AppendPackage(B.AsByteArray(32), 32, 3); // [3..34] B
	buf.ByteBuffer[35] = 1; // [35..37] consts
	buf.ByteBuffer[36] = 7; // fixed g.AsByteArray(1) // [35..37] consts
	buf.ByteBuffer[37] = 32; // [35..37] consts
	buf.AppendPackage(N.AsByteArray(32), 32, 38); // // [37, 38..69] modN
	buf.AppendPackage(s.AsByteArray(), s.GetNumBytes(), 70); // [70..101] salt
	buf.AppendPackage(unk3.AsByteArray(16), 16, 102); // [102..118] unk3

	buf.BufferAuth_send(119);
	return true;
}

bool AuthServer::_HandleLogonProof()
{
	ASLog.DebugLog("Entering _HandleLogonProof\n");

	ByteBufferSocket buf;
	buf.BufferAuth_recv(sizeof(sAuthLogonProof_C));
	// Read the packet
	sAuthLogonProof_C lp;

	return 0;
}

bool AuthServer::_HandleReconnectChallenge()
{
	ASLog.DebugLog("Entering _HandleReconnectChallenge\n");
	return 0;
}

bool AuthServer::_HandleReconnectProof()
{
	ASLog.DebugLog("Entering _HandleReconnectProof\n");
	return 0;
}

bool AuthServer::_HandleRealmList()
{
	ASLog.DebugLog("Entering _HandleRealmList\n");
	return 0;
}

void ByteToALC_c(sAuthLogonChallenge_C ALCC, ByteBufferSocket buff)
{

}