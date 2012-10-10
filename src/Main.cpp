/*
*http://incpp.blogspot.com/2010/11/openssl.html
*/
#include "Main.h"
#include "Logs\Logs.h"
#include "Common.h"
//#include <mysql\mysql.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "Auth\sha1.h"
#include "Auth\BigNumber.h"
#include "Auth\AuthCodes.h"


//const int s_BYTE_SIZE = 32;
int WS_PORT = 7000;
int RS_PORT = 3724;

WSADATA data;
SOCKET hSocket, newSocket;
sockaddr_in sock_in;

//const int WS_BUFFER_SIZE = 30*1024;
char buffer[0x1000], buffer2[0x1000];

struct TSRP6
{
	std::string Login;
	unsigned char LP_HASH[SHA_DIGEST_LENGTH];
    unsigned char SRP6_N[31];
    unsigned char SRP6_Salt[31];
    unsigned char SRP6_b[19];
    unsigned char SRP6_B[31];
    unsigned char SRP6_v[31];
    unsigned char SRP6_g;
    unsigned char SRP6_A[31];
    unsigned char SRP6_M1[19];
    unsigned char SRP6_M2[19];
    unsigned char Session[39];
};

struct CMD_AUTH_LOGON_CHALLENGE_c
{
	std::string AccountName;
    std::string ClientLang;
	std::string UserIP;
    unsigned short ClientBuild;

	TSRP6 data;
};

void main()
{
	CMD_AUTH_LOGON_CHALLENGE_c AUTH;
	std::string LP;
	std::string HashBuffer;
	
	if (WSAStartup(0x0202, &data)==0)
	{
		hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (hSocket == INVALID_SOCKET)
		{
			std::cout<<"ERROR OPEN SOCKET\n";
		}

		sock_in.sin_family = AF_INET;
		sock_in.sin_port = htons(RS_PORT);
		sock_in.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");

		int i = bind(hSocket, (sockaddr*)(&sock_in), sizeof(sock_in));
		if (i==0)
		{
			MainLog("Bind port: %d\n", RS_PORT);
		}

		int l = listen(hSocket, SOMAXCONN);
		if (l==0)
		{
			MainLog("Listen port: %d\n", RS_PORT);
		}

		int k = sizeof(sock_in);
		newSocket = accept(hSocket, (sockaddr*)&sock_in, &k);
		if (newSocket==INVALID_SOCKET)
		{
			std::cout<<"error";
		}
		else
		{
			/// Logon Challenge command handler
			//MainLog("socket true\n");
			int bytesReceived = recv(newSocket, buffer, sizeof(buffer), 0);

			AUTH.ClientBuild = buffer[11] + 256*buffer[12];
			for (int i = 24; i >= 21; i--)
			{
				AUTH.ClientLang +=  buffer[i];
			}
			for (int i = 1; i <= buffer[33]; i++)
			{
				AUTH.AccountName += buffer[34+i-1];
			}

			MainLog("AUTH_LOGON_CHALLENGE ['%st'] ['%st'] ['%us']\n", AUTH.AccountName, AUTH.ClientLang, AUTH.ClientBuild);
			// check Client Build
			if (UPDATEFIELDS_BUILD != AUTH.ClientBuild)
			{
				MainLog("Version mismatch. Upgrade the DWCore.");
				buffer[0] = CMD_AUTH_LOGON_CHALLENGE;
				buffer[1] = 0;
				buffer[2] = AUTH_INVALID_VERSION;
				buffer[3] = 0;
				send(newSocket, buffer, 4, 0);
				exit(0);
			}

			// clear send buffer
			for (int i = 0; i <= 119; i++)
			{
				buffer[i] = 0;
			}

			// подготавливаем данные
			AUTH.data.Login = AUTH.AccountName;
			// sha1
			LP = AUTH.AccountName;
			LP += ":";
			LP += AUTH.AccountName;
			
			// реализация SPR-6 ИЗ КОДА МАНГОСА
			BigNumber N, s, g, v;
			BigNumber b, B;
			BigNumber K;
			
			N.SetHexStr("894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7");
			g.SetDword(7);
			
			s.SetRand(32 * 8);
			
			BigNumber I;
			I.SetHexStr(LP.c_str());

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
			/*const char *v_hex, *s_hex;
			v_hex = v.AsHexStr();
			s_hex = s.AsHexStr();*/
			
			b.SetRand(19 * 8);
			BigNumber gmod = g.ModExp(b, N);
			B = ((v * 3) + gmod) % N;

			BigNumber unk3;
			unk3.SetRand(16 * 8);


			// header
			buffer[0] = CMD_AUTH_LOGON_CHALLENGE;
			buffer[1] = 0x0000;//AUTH_OK;
			// unk
			buffer[2] = 0;
			// [3..34] B
			unsigned char * _Bbuff = B.AsByteArray(32);
			for (int i = 3; i<=34; i++)
			{
				buffer[i] = _Bbuff[i-3];
			}
			// [35..37] consts
			buffer[35] = 1;
			buffer[36] = 7;
			// [37, 38..69] modN
			buffer[37] = 32;
			unsigned char * _Nbuff = N.AsByteArray(32);
			for (int i = 38; i<=69; i++)
			{
				buffer[i] = _Nbuff[i-38];
			}
			// [70..101] salt
			unsigned char * _sbuff = s.AsByteArray();
			//std::cout<<"\n";
			for (int i = 70; i <= 101; i++)
			{
				buffer[i] = _sbuff[i-70];
				//std::cout<<buffer[i];
			}
			//std::cout<<"\n";
			// [102..118] unk3
			for (int i = 0; i <= 15; i++)
			{
				buffer[102 + i] = 0;
			}

			send(newSocket, buffer, 119, 0);

			int bytesReceived2 = recv(newSocket, buffer2, sizeof(buffer2), 0);

			for (int i = 0; i<120; i++)
			{
				std::cout<<buffer2[i];
			}
			std::cout<<"\n";
		}

		//closesocket(hSocket); // закрываем сокет
		//WSACleanup(); // освобождение
	}

	closesocket(hSocket); // закрываем сокет
	WSACleanup(); // освобождение
	system("pause");

}