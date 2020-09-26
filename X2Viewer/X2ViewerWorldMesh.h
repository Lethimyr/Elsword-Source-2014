#pragma once

class CX2ViewerWorldMesh : public CX2ViewerObject
{
	public:
		CX2ViewerWorldMesh(void);
		virtual ~CX2ViewerWorldMesh(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		bool			SetMesh( const WCHAR* wstrPullName );
		CKTDGMatrix*	GetMatrix(){ return m_pMatrix; }

		void			Reset(){ ReleaseMesh(); }

	private:
		void	ReleaseMesh();

	private:
		CKTDXDeviceXMesh*								m_pXMesh;
		CKTDGMatrix*									m_pMatrix;
};
