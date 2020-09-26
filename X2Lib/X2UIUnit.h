#pragma once

typedef srutil::delegate0<void> UINPCStateCBFunc;
//{{ seojt // 2009-1-13, 17:22
class CX2UIUnit;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2UIUnit>    CX2UIUnitPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2UIUnit>    CX2UIUnitPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 17:22


class CX2UIUnit : public CKTDGObject
{
	public:
		struct UINPCStateData
		{
			string				m_StateTable;
			int					m_PlayCount;
			float				m_fStateTime;

			string				m_Lua_StateStart;
			string				m_Lua_FrameMove;
            string				m_Lua_EventProcess;
			string				m_Lua_StateEnd;

			UINPCStateData()
			{
				Init();
			}

			virtual void Init()
			{
				m_StateTable		= "";
				m_PlayCount			= 0;
				m_fStateTime		= 0.0f;

				m_Lua_StateStart	= "";
				m_Lua_FrameMove		= "";
				m_Lua_EventProcess	= "";
				m_Lua_StateEnd		= "";
			}
			UINPCStateData& operator=( const UINPCStateData& data )
			{
				m_StateTable		= data.m_StateTable;
				m_PlayCount			= data.m_PlayCount;
				m_fStateTime		= data.m_fStateTime;

				m_Lua_StateStart	= data.m_Lua_StateStart;
				m_Lua_FrameMove		= data.m_Lua_FrameMove;
				m_Lua_EventProcess	= data.m_Lua_EventProcess;
				m_Lua_StateEnd		= data.m_Lua_StateEnd;

				return *this;
			}
		};

	private:
		//CX2UIUnit() { ASSERT( !"invalid" ); }
		CX2UIUnit( const wstring& pFileName );
		virtual ~CX2UIUnit(void);

	public: 
		static CX2UIUnitPtr CreateUIUnit( const wstring& fileName )
		{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            CX2UIUnitPtr pObject( new CX2UIUnit( fileName ) );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            CX2UIUnitPtr pObject( new CX2UIUnit( fileName ), CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            return pObject;
		}//CreateUIUnit()

		//static void DeleteKTDGObject( CX2UIUnit* pObject )  
		//{ 	
		//	if( NULL != pObject )
		//	{
		//		CKTDGObject::DeleteKTDGObject( pObject ); 
		//	} 
		//}





		void Init();

		virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
//{{ robobeg : 2008-10-28
		//virtual HRESULT		OnFrameRender();'
        virtual void        OnFrameRender_Draw();
//}} robobeg : 2008-10-28

		void StateChange( string stateID );
//{{ robobeg : 2008-10-18
		//void SetShow( bool bShow ){ m_bShow = bShow; m_pTalkBoxMgr->Clear(); m_pTalkBoxMgr->SetShowObject(bShow); }
        virtual void NotifyShowObjectChanged();
//}} robobeg : 2008-10-18

		D3DXVECTOR3 GetPos() { return m_Pos; }
		void SetPos( D3DXVECTOR3 val ) { m_Pos = val; }

		D3DXVECTOR3 GetRotate() { return m_Rotate; }
		void SetRotate( D3DXVECTOR3 val ) { m_Rotate = val; }

		void SetCameIn(bool val) { m_bCameIn = val; }
		bool GetCameIn() const { return m_bCameIn; }
		bool IsComingIn();
		
		void ClearNextStateID() { m_vecNextStateID.clear(); }
		string PopNextStateID();
		void PushNextStateID( string stateID ) { m_vecNextStateID.push_back( stateID ); }

		const char* GetWaitStateID()		{ return m_WaitStateID.c_str();		}
		const char* GetComeInStateID()		{ return m_ComeInStateID.c_str();	}
		const char* GetBuyInStateID()		{ return m_BuyInStateID.c_str();	}
		const char* GetQuestInStateID()	    { return m_QuestInStateID.c_str();	}
		const char* GetMenuInStateID()		{ return m_MenuInStateID.c_str();	}


		

	protected:
		virtual void				CommonStateStart();
		virtual void				CommonFrameMove();
		virtual void				CommonEventProcess();
		virtual void				CommonStateEnd();

		void						LoadTalkBox();
		wstring							m_ScriptFileName;
		KLuaManager						m_LuaManager;
//{{ robobeg : 2008-10-18
		//bool							m_bShow;
//}} robobeg : 2008-10-18

		UINPCStateData					m_NowState;
		string							m_WaitStateID;
		string							m_ComeInStateID;
		string							m_BuyInStateID;
		string							m_QuestInStateID;
		string							m_MenuInStateID;
		

		float							m_fTime;
		float							m_fElapsedTime;
		float							m_fStateTime;
		int								m_StatePlayCount;

		D3DXVECTOR3						m_Pos;		
		D3DXVECTOR3						m_Rotate;		
		D3DXVECTOR3						m_Scale;

        /// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        /// - jintaeks on 2009-01-12, 16:51
		CKTDGXSkinAnim*					m_pXSkinAnim;
		CKTDXDeviceXSkinMesh*			m_pMesh;
		CKTDXDeviceXET*					m_pChangeTexXET;
		CKTDXDeviceXET*					m_pMultiTexXET;
		CKTDXDeviceXET*					m_pAnimXET;	

		D3DXVECTOR2						m_TexStage0Speed;
		D3DXVECTOR2						m_TexStage1Speed;
		D3DXVECTOR2						m_TexStage2Speed;

		CX2TalkBoxManager*				m_pTalkBoxMgr;

		bool							m_bCameIn;


		vector< string >				m_vecNextStateID;		

};
