/*
*
*/

#include "..\Common.h"
#include "..\logs\Logs.h"
#include "BigNumber.h"
#include "sha1.h"
#include "BufferedSocket.h"

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
	//unsigned char I[1];
	std::string I;

} sAuthLogonChallenge_C;


class AuthServer
{
	public:
		const static int s_BYTE_SIZE = 32;

		AuthServer();
		~AuthServer();

		void _SetVSFields(const std::string& rI);
		void SendProof(Sha1Hash sha);

		bool _HandleLogonChallenge();
		bool _HandleLogonProof();
        bool _HandleReconnectChallenge();
        bool _HandleReconnectProof();
        bool _HandleRealmList();

	private:
		ByteBufferSocket buf;
        BigNumber N, s, g, v;
        BigNumber b, B;
        BigNumber K;
        BigNumber _reconnectProof;

        bool _authed;

        std::string _login;
		unsigned short _build;
	    std::string _os;

		Log ASLog;

		void BuffToAUTH_LOGON_CHALLENGE_C();
		AUTH_LOGON_CHALLENGE_C alc;
};