// test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <stdio.h>
#include "stdafx.h"

#include "../KPatcherLib/patch.h"
#include "../KPatcherLib/curlsession.h"


bool TotalProgress(SProgressInfo * info)
{	
	printf("%s %i / %i\n",info->currentfilename.c_str(),info->currentreadlength,info->currentlength);
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	
	KCurlSession session;
	Komfile kom;
	
	kom.Open(&session,"http://116.120.238.38/cp/GrandChase/KR_SEASON2_TEST/test.kom");
	
	KSubfile *subfile = kom.GetSubFile("actionscript.stg");
	subfile->WriteDecompressed("c:\\actionscript.stg");
	//kom.Save("c:\\test2.kom");
	return 0;
}

