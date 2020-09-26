#pragma once

#include <string>

#include "KSingleton.h"

class KNetUtil
{

public:
	KNetUtil();
	virtual ~KNetUtil();

	void WSAStartUp();
	void WSACleanUp();

	std::string GetHostName();
	std::string GetHostIP();

	int GetRandomPort();

};

KDeclareGlobalVal( KNetUtil );