#pragma once

class CFCSort
{
	public:
		CFCSort(void);
		~CFCSort(void);

		bool SortMesh( const WCHAR* pFileName );
		void SaveMesh( const WCHAR* pFileName, bool bExpendName );
		void SaveMeshAuto( const WCHAR* pPath, const WCHAR* pFileName, bool bExpendName );

	private:
		HRESULT SortFrameIn( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame );
		bool	ListSort( CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame);

		CKTDXDeviceXSkinMesh*							m_pMesh;
		vector<CKTDXDeviceXSkinMesh::MultiAnimFrame*>	m_TempList;
};
