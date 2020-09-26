#pragma once

class CListSort
{
public: 
	bool operator()( const CKTDXDeviceXSkinMesh::MultiAnimFrame* p, const CKTDXDeviceXSkinMesh::MultiAnimFrame* q ) const
	{ 
		if( strcmp(p->Name,q->Name) < 0 )
			return true;
		else
			return false;
	}
};

class CXFileFrameSort : public CKTDXStage
{
	public:
		CXFileFrameSort(void);
		~CXFileFrameSort(void);

		void OpenFile();
		void OpenMesh();
		void SortFrame();
		HRESULT SortFrameIn( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame );

		bool ListSort( CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame );

		void SaveFile();

	private:
		OPENFILENAME	m_OFN;
		wstring			m_wsFileName;
		wstring			m_wsPullFileName;
		string			m_strFileName;
		string			m_strPullFileName;

		CKTDXDeviceXSkinMesh*	m_pSkinMesh;
		CKTDXDeviceXMesh*		m_pMesh;

		vector<CKTDXDeviceXSkinMesh::MultiAnimFrame*> m_TempList;
};
