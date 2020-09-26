#pragma once

class CX2CheckSumManager
{
	public:

		CX2CheckSumManager(void);
		~CX2CheckSumManager(void);

		void Handler_EGS_CHECK_SUM_REQ( bool bCheckFile = false );

	protected:

		bool OpenScript( const WCHAR* pFileName );
		void CheckSumFileProcess();

		vector< wstring >		m_vecCheckSumFileName;
		map< wstring, wstring > m_mapCheckSum;
};
