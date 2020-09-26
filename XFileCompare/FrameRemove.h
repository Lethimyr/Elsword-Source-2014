#pragma once

class CFrameRemove
{
	public:
		CFrameRemove(void);
		~CFrameRemove(void);

		void SetFile( wstring fileName );
		bool RemoveFrameName( wstring frameName );
		bool RemoveFrameName( vector<wstring> frameNameList );
		void SaveMesh( const WCHAR* pFileName, bool bExpendName );

	private:
		HRESULT RemoveFrameIn( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, vector<wstring>& frameNameList );

		CKTDXDeviceXSkinMesh* m_pXSkinMesh;
        
};
