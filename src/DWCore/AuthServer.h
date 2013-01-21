/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: AuthServer.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#ifndef _AUTHSERVER_H
#define _AUTHSERVER_H

#include "Common.h"
#include "BigNumber.h"
#include "sha1.h"
#include "ByteBuffer.h"
#include "BufferedSocket.h"

class AuthServer: public BufferedSocket
{
	public:
		const static int s_BYTE_SIZE = 32;

		AuthServer();
		~AuthServer();
		void OnAccept();
		void OnRead();
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
		std::string _safelogin;
		uint16 _build;
	    std::string _os;
		ACE_HANDLE patch_;
		Log ASLog;
};

#endif