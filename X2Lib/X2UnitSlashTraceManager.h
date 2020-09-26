#pragma once



#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
class CX2UnitSlashTraceManager
{
public: 
	enum SLASH_TRACE_CONDITION
	{
		STC_NONE,
		STC_RENA_NATURE_FORCE,
	};

	struct UnitSlashTraceTiming
	{
		int		m_iSlashTraceIndex;
		float	m_fStartAnimationTime;
		float	m_fEndAnimationTime;
		CKTDGSlashTrace::SLASH_TRACE_TYPE m_eSlashTraceType;

		UnitSlashTraceTiming() 
		: m_iSlashTraceIndex( -1 )
		, m_fStartAnimationTime( 0.f )
		, m_fEndAnimationTime( 0.f )
		, m_eSlashTraceType( CKTDGSlashTrace::STT_NONE )
		{
		}

		UnitSlashTraceTiming( int iSlashTraceIndex, float fStartAnimationTime, float fEndAnimationTime, CKTDGSlashTrace::SLASH_TRACE_TYPE eSlashTraceType )
		: m_iSlashTraceIndex( iSlashTraceIndex )
		, m_fStartAnimationTime( fStartAnimationTime )
		, m_fEndAnimationTime( fEndAnimationTime )
		, m_eSlashTraceType( eSlashTraceType )
		{
		}

	}; // struct UnitSlashTraceTiming


	struct UnitSlashTraceData
	{
		UnitSlashTraceData()
		: m_pSlashTrace( NULL )
		, m_pSlashTraceTip( NULL )
		, m_pFrame_TRACE_START( NULL )
		, m_pFrame_TRACE_END( NULL )
		{
		}

		~UnitSlashTraceData()
		{
		}

		void Delete()
		{
			m_pFrame_TRACE_START = NULL;
			m_pFrame_TRACE_END = NULL;

			SAFE_DELETE_KTDGOBJECT( m_pSlashTrace );
			SAFE_DELETE_KTDGOBJECT( m_pSlashTraceTip );
		}

		void AddSlashTraceByType( CKTDGSlashTrace::SLASH_TRACE_TYPE eType, const D3DXVECTOR3& up, const D3DXVECTOR3& down, const D3DXVECTOR3& tipDown, CX2GameUnit* pGameUnit );


	public:
		CKTDGSlashTrace* m_pSlashTrace;
		CKTDGSlashTrace* m_pSlashTraceTip;

		CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pFrame_TRACE_START;
		CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pFrame_TRACE_END;

		D3DXCOLOR	m_coSlashTrace;
		D3DXCOLOR	m_coSlashTraceHyper;
		D3DXCOLOR	m_coSlashTraceTip;
		D3DXCOLOR	m_coSlashTraceTipHyper;


	}; // struct UnitSlashTraceData



private:
	CX2UnitSlashTraceManager( CX2GameUnit* pGameUnit, CKTDGXSkinAnimPtr pUnitSkinAnim )  
		: m_pGameUnit( pGameUnit )
		, m_pUnitSkinAnim( pUnitSkinAnim )
		, m_fSlashTraceTipWidth( 0 )
		, m_bShow( true )
		, m_bEnable( true )
	{
		m_vecUnitSlashTraceData.reserve( 4 );
	}

	~CX2UnitSlashTraceManager()
	{
		BOOST_TEST_FOREACH( UnitSlashTraceData&, slashTraceData, m_vecUnitSlashTraceData )
		{
			slashTraceData.Delete();
		}
	}

public: 
	static void DeleteUnitSlashTraceManager( CX2UnitSlashTraceManager*& pUnitSlashTraceManager )
	{
		SAFE_DELETE( pUnitSlashTraceManager );
		pUnitSlashTraceManager = NULL;
	}

	static CX2UnitSlashTraceManager* CreateUnitSlashTraceManager( CX2GameUnit* pGameUnit, CKTDGXSkinAnimPtr pUnitSkinAnim )
	{
		CX2UnitSlashTraceManager* pUnitSlashTrace = new CX2UnitSlashTraceManager( pGameUnit, pUnitSkinAnim );
		return pUnitSlashTrace;
	}


	void OnStateStart();
	void OnFrameMove( double fTime, float fElapsedTime );


	void AddSlashTraceTiming( int iSlashTraceIndex, float fStartAnimationTime, float fEndAnimationTime, CKTDGSlashTrace::SLASH_TRACE_TYPE eSlashTraceType )
	{
		m_mapSlashTraceTiming[ iSlashTraceIndex ] = UnitSlashTraceTiming( iSlashTraceIndex, fStartAnimationTime, fEndAnimationTime, eSlashTraceType );
	}

	void ClearSlashTraceTiming()
	{
		m_mapSlashTraceTiming.clear();
	}

	UINT GetUnitSlashTraceDataSize() const { return m_vecUnitSlashTraceData.size(); }

	void AddSlashTraceData( const wstring& topBoneName, const wstring& bottomBoneName, 
		const D3DXCOLOR& coSlashTrace, const D3DXCOLOR& coSlashTraceHyper, const D3DXCOLOR& coSlashTraceTip, const D3DXCOLOR& coSlashTraceTipHyper,
		const int vertexNum, float fSlashTraceDisableTime = 0.4f, int iSlashTraceDrawCount = 1,
		const int vertexNumTip = 0, float fSlashTraceTipDisableTime = 0.4f, int iSlashTraceTipDrawCount = 1, 
		bool bTexturedSlashTrace = false, const float fTextureStride = 0.f, const wstring& slashTraceTextureName = L"",
		CKTDGSlashTrace::CURVE_TYPE eCurveType = CKTDGSlashTrace::CT_SMOOTH_CURVE );


	void SetSlashTraceTipWidth(const float val) { m_fSlashTraceTipWidth = val; }

	bool GetShow() const { return m_bShow; }
	void SetShow( bool bShow )
	{
		m_bShow = bShow;

		for( int i=0; i<(int)m_vecUnitSlashTraceData.size(); i++ )
		{
			UnitSlashTraceData& slashTraceData = m_vecUnitSlashTraceData[i];

			if( NULL != slashTraceData.m_pSlashTrace )
			{
				slashTraceData.m_pSlashTrace->SetShowObject( bShow );
			}

			if( NULL != slashTraceData.m_pSlashTraceTip )
			{
				slashTraceData.m_pSlashTraceTip->SetShowObject( bShow );
			}
		}
	}

	bool GetEnable() const { return m_bEnable; }
	void SetEnable(const bool val) { m_bEnable = val; }


private:
	CX2GameUnit* m_pGameUnit;
	CKTDGXSkinAnimPtr m_pUnitSkinAnim;
	std::map< int, UnitSlashTraceTiming > m_mapSlashTraceTiming;
	std::vector< UnitSlashTraceData > m_vecUnitSlashTraceData;

	float m_fSlashTraceTipWidth;
	bool m_bShow;		// owner unit의 보임 상태에 따라 
	bool m_bEnable;		// 그래픽 옵션 설정에 따라 보이거나 안보이거나

};


#endif UNIT_SLASH_TRACE_MANAGER_TEST
