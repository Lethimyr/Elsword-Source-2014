const CRect g_iBannerRect = CRect(427, 177-123, 427+353, 177-123+232);
const WCHAR g_pcBannerLink[] = _T("http://els.ztgame.com/p2p/p2p.shtml");

#ifdef _SERVICE_
#ifdef _OPEN_TEST_
	const char g_pcGameServerListXmlAddr[] = "testels.ztgame.net";
	const char g_pcGameServerListXmlPath[] = "autopatchelstest/srvlist.xml";
#else _OPEN_TEST_
	const char g_pcGameServerListXmlAddr[] = "updateels.ztgame.net";
	const char g_pcGameServerListXmlPath[] = "autopatchels/srvlist.xml";
#endif _OPEN_TEST_
#else _SERVICE_
	const char g_pcGameServerListXmlAddr[] = "192.168.71.235";
	const char g_pcGameServerListXmlPath[] = "patch/srvlist_test.xml";
#endif _SERVICE_
