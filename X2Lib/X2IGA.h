//{{ kimhc // 2010.3.17 //	광고 노출도
#ifdef	IGA_TEST

#pragma once

#define ELAPSED_TIME_TO_CHECK 0.5

class CX2Camera;

class CX2IGA
{
	DeclareSingleton( CX2IGA );

public:	
	struct IGA_INFO
	{
		D3DXVECTOR3 m_vPoint[4];
		D3DXVECTOR3 m_vNormal;

		void SetPoint( D3DXVECTOR3& vLB, D3DXVECTOR3& vLT, D3DXVECTOR3& vRT, D3DXVECTOR3& vRB )
		{
			m_vPoint[0] = vLB;
			m_vPoint[1] = vLT;
			m_vPoint[2] = vRT;
			m_vPoint[3] = vRB;
		}
	};

	enum IGA_STATE
	{
		IS_NONE,
		IS_INIT,
		IS_SET_ENV,
		IS_CLEAR_IMG,
		IS_SET_USER,
		IS_DOWN_LOAD_TEX,
		IS_START,
		IS_END,
	};

	CX2IGA() {}

	//# 프로그램 생성시 딱한번만 호출
	void Init();

	//# 프로그램 종료시 딱한번만 호출
	void Clear();

	//# 캐릭터 생성시 정보등록
	// - 광고에 대한 통계 함수
	void SetUser( wstring& wstrID, int iAge, bool bMan );

	//# 외부의 애드서버로 부터 광고이미지 정보를 받아서 저장하는 함수
	void DownLoadTexture();

	//# 마을별 광고판의 위치 정보가 하드코딩 되어 있는 함수
	void CreateIgaInfoInVillage( int iVillageID );

	//# 거래광장의 광고판의 위치 정보가 하드코딩 되어 있는 함수
	void CreateIgaInfoInMarket();

	//# 광고노출도 정보를 보내기 전에 한번만 호출하는 함수
	//  ┗ 단, End() 호출 후에 다시 광고노출 정보를 보내기 위해서는 다시 한번 호출을 해야함
	void Start();

	//# 광고노출도 정보를 다 보낸 후 호출하는 함수
	void End();

	//# 화면 광고노출도 서버로 보내기
	//  ┗ 마을 or 광장에서 ELAPSED_TIME_TO_CHECK에 지정된 초마다
	void ProcessTracking( CX2Camera* pCamera, float fElapsedTime );

	//# 광고노출도를 계산하는 함수
	void Track( IGA_INFO& IgaInfo, int iIndex, CX2Camera* pCamera );

	//# 광고판의 위치정보와 normal정보를 저장
	//  ┗m_vecIgaInfo에 저장
	void InitAdCoord( D3DXVECTOR3& vLT, D3DXVECTOR3& vLB, D3DXVECTOR3& vRT, D3DXVECTOR3& vRB );

public:
	std::vector<IGA_INFO>	m_vecIgaInfo;
	static IGA_STATE		m_eNowState;
	IGA_TEXTURE				m_IgaTexture;
	string					m_strInvid;
	string					m_strFileName;

	static float			m_fElapsedTime;

};

DefSingletonInline( CX2IGA );
#endif	IGA_TEST
//}} kimhc // 2010.3.17 //	광고 노출도