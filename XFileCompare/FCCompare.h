#pragma once

class CFCCompare
{
	public:
		CFCCompare(void);
		~CFCCompare(void);

		void SetFirstFileName( const WCHAR* pFileName );
		void SetSecondFileName( const WCHAR* pFileName );

		bool Compare();

		bool GetCompareResult(){ return m_bSame; }
		int GetFirstFrameNum();
		int GetSecondFrameNum();

		int GetFirstMeshLogSize() { return (int)m_FirstMeshLog.size(); }
		int GetSecondMeshLogSize() { return (int)m_SecondMeshLog.size(); }

		wstring GetFirstMeshLogByIndex( int index );
		wstring GetSecondMeshLogByIndex( int index );

	private:
		void CompareHierarchy( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame , CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame2 );
		void ComputeHierarchyExplain( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame , CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame2  );

		wstring m_FirstFileName;
		wstring m_SecondFileName;

		vector<wstring>			m_FirstMeshLog;
		vector<wstring>			m_SecondMeshLog;

		bool					m_bSame;
		CKTDXDeviceXSkinMesh*	m_pFirstMesh;
		CKTDXDeviceXSkinMesh*	m_pSecondMesh;
};
