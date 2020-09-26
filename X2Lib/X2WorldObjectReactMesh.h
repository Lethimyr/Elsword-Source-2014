#pragma once

class CX2WorldObjectReactMesh : public CX2WorldObjectSkinMesh
{
	public:
		struct ReactMeshObjectData : public CX2WorldObjectSkinMesh::SkinMeshObjectData
		{
			wstring	reactAnim;

			float	fHP;
            float	fReactDistance;
            
			void Init()
			{
				CX2WorldObject::ObjectData::Init();

				objectType	= CX2WorldObject::OT_REACT_MESH;

				reactAnim	= L"";

				fHP				= 0.0f;
				fReactDistance	= 0.0f;
			}

			ReactMeshObjectData()
			{
				Init();
			}
		};



	private:
		CX2WorldObjectReactMesh( ReactMeshObjectData* pReactMeshObjectData );
		~CX2WorldObjectReactMesh(void);


	public: 
		static CX2WorldObjectReactMesh* CreateWorldObjectReactMesh( ReactMeshObjectData* pReactMeshObjectData )
		{
			return new CX2WorldObjectReactMesh( pReactMeshObjectData );
		}
		

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

		void React( D3DXVECTOR3 pos );

	protected:
		ReactMeshObjectData*	m_pReactMeshObjectData;
		bool					m_bNowReact;
};
