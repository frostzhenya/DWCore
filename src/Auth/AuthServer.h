/*
*
*/

#include "..\Common.h"
#include "..\logs\Logs.h"
#include "BigNumber.h"
#include "sha1.h"
#include "BufferedSocket.h"

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
        BigNumber N, s, g, v;
        BigNumber b, B;
        BigNumber K;
        BigNumber _reconnectProof;

        bool _authed;

        std::string _login;
		unsigned short _build;
	    std::string _os;

		Log ASLog;
};