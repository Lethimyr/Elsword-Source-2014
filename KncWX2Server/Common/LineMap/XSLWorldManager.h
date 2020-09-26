/** @file   X2WorldManager.h
    @date   jintaeks on 2008-10-21, 14:03
            - preprocessing data를 file로 기록하는 기능을 추가하다.
                background loading작업시 게임 객체를 world에 배치하기 전에 필요한 정보 - bounding
                sphere radius 같은 - 를 파일로 write하는 기능을 추가하다.
    @date   jintaeks on 2008-10-24, 13:35
            - culling 정보를 확인하기 위하여 KHelpSystem을 추가하다.
*/
#pragma once

class CXSLWorldManager
{
    public:

#ifdef KHELP_SYSTEM_TEST // 2008-10-24
        DECLARE_HELP_SYSTEM( CXSLWorldManager );
#endif // KHELP_SYSTEM_TEST // 2008-10-24

        //{{ seojt // 2008-10-21, 14:14
        /** @enum   PREPROCESSING_INFO_TYPE
            @brief  WORLD_PREPROCESSING_INFO가 사용하는 전처리 데이터의 종류를 나타낸다.
            @date   jintaeks on 2008-10-21, 14:19 */
        enum PREPROCESSING_INFO_TYPE
        {
            X2WM_PPIT_UNKNOWN       = -1,
            X2WM_PPIT_XMESH         = 0,
            X2WM_PPIT_XMESHLOD      = 1,
            X2WM_PPIT_XSKIN_MOTION  = 2,
            X2WM_PPIT_XSKIN_MODEL   = 3,
        };//enum PREPROCESSING_INFO_TYPE

        struct WORLD_PREPROCESSING_INFO
        {
            PREPROCESSING_INFO_TYPE     m_eInfoType;        ///< 전처리 데이터의 종류
            std::wstring                m_wstrName;         ///< 메시 파일 이름
            D3DXVECTOR3                 m_vCenter;          ///< 메시의 local center
            float                       m_fBSphereRadiua;   ///< 메시 bounding sphere의 반지름
        };//struct WORLD_PREPROCESSING_INFO
        //}} seojt // 2008-10-21, 14:14

	public:
		CXSLWorldManager();
		virtual ~CXSLWorldManager(void);

		CX2World* CreateWorld( CX2World::WORLD_ID worldID, KLuabinder* pKLuabinder = NULL );
		bool OpenScriptFile( const WCHAR* pFileName );

        //{{ seojt // 2008-10-22, 10:23
        /// 월드 객체를 파괴하기 바로 전에 호출된다.
        void OnPreDestroyWorld( CX2World* pWorld_ );
        //}} seojt // 2008-10-22, 10:23

        CX2World* GetWorldLatest() { return m_pX2WorldLatestRef; }

		bool AddWorldData_LUA();
        //{{ seojt // 2008-10-21, 15:21
        /// 월드 매니저가 유지하는 객체의 전처리 정보를 설정한다.
        bool SetPreprocessingData_LUA();
        //}} seojt // 2008-10-21, 15:21

	public: 
		const std::vector<CX2World::WorldData*>& GetWorldDataList() { return m_WorldDataList; }
		CX2World::WORLD_ID GetWorldIDUsingLineMapFileName( const WCHAR* pLineMapFileName );
		const CX2World::WorldData* GetWorldData( CX2World::WORLD_ID eWorldID );

        ///-----------------------------------------------------------------------------
        //{{ 전처리 데이터 관련 interface의 시작 - jintaeks on 2008-10-21, 14:25
        //
        void    EnableWritingPreprocessingData( bool bEnable_ );
        bool    IsWritingPreprocessingData() const { return m_bWritingPreprocessingData; }
        bool    WritePreprocessingData( const WORLD_PREPROCESSING_INFO& preInfo_ );
        //
        //}} 전처리 데이터 관련 interface의 끝
        ///-----------------------------------------------------------------------------

#ifdef BACKGROUND_LOADING_TEST // 2008-10-18
        /// 기존의 CreateWorld()함수의 thread 가능한 버전이다.
        /// 리소스 로딩을 쓰레드에서 진행하기 위해, 리소스 로딩 코드를 쓰레드가 관리하는
        /// 리소스 요청 큐에 쌓는 작업을 한다.
        /// 실제의 리소스 로딩은 쓰레드에서 이루어진다.
        /// - jintaeks on 2008-10-18, 12:32
		CX2World* ThreadRequest_CreateWorld( CX2World::WORLD_ID worldID_, KLuabinder* pKLuabinder_ = NULL );
#endif // BACKGROUND_LOADING_TEST // 2008-10-18

    //{{ seojt // 2008-10-24, 13:53
    public:
        void Debug_RenderHelp( int ix_, int iy_ );
    //}} seojt // 2008-10-24, 13:53

	//{{ 2007. 9. 7  최육사  상속을 위한 protected선언
	protected:
	//}}
		CRITICAL_SECTION				m_csWorldManager;
		std::vector<CX2World::WorldData*>	m_WorldDataList;
        CX2World*                       m_pX2WorldLatestRef;    ///< 가장 최근에 생성한 월드 객체에 대한 포인터

        //{{ 전처리 데이터 관련 시작 - jintaeks on 2008-10-21, 14:25
        bool                            m_bWritingPreprocessingData;
        std::string                     m_strPreprocessingDataFile;
        //}} 전처리 데이터 관련 끝
};//class CX2WorldManager


