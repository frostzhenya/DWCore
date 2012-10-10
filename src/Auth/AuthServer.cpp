/*
* code in mangos
*/

#include "AuthServer.h"

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
