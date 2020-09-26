#ifndef __XFileExp_FRAME__H
#define __XFileExp_FRAME__H

#include "XFileExp.h"
#include "CXExpSkinData.h"

typedef struct 
{
    CXExpSkinData *m_pSkinData;
    LPDIRECTXFILESAVEOBJECT m_pxofsave;
    
} _PS;


typedef struct 
{
    float fClipU;
    float fClipV;
    float fClipW;
    float fClipH;
} SCropInfo;


class SkinWeightBuf
{
public:
    SkinWeightBuf() {}
    ~SkinWeightBuf() {}

public:
    float m_Weight;
    char m_Bone[256];
};

class SkinWeightInx
{
public:
    SkinWeightInx() {}
    ~SkinWeightInx() {}

public:
    DWORD m_Index;
    float m_Weight;
};

class SkinWeightData
{
public:
    SkinWeightData() {}
    ~SkinWeightData() 
    {
        for(int i=0; i<m_vecSkinIndex.size(); ++i)
            delete m_vecSkinIndex[i];        
    }

public:
    char m_Name[256];
    std::vector<SkinWeightInx *> m_vecSkinIndex;
    D3DMATRIX m_OffsetMat;
    INode *m_pBoneNode;

    DWORD m_cbData;
    PBYTE m_pbData;
};

class CFaceLIst
{
public:
    CFaceLIst() {}
    ~CFaceLIst() {}

public:
    DWORD m_nId;
    Face *pFace;
    DWORD m_VertexInx[3];
    DWORD m_FaceMaterial;
    DWORD m_SmoothGroup, m_MtlId;
};

class CRefVertex
{
public:
    CRefVertex() {}
    ~CRefVertex() 
    {
        for(int i=0; i<m_SWList.size(); ++i)
            delete m_SWList[i];        
    }

public:
    DWORD nId;        // 버텍스 id (중복버텍스일 경우 기본버텍스의 id에서 증가한다.)
    DWORD refVertex;  // 참조되는 버텍스 id(범위 : 기본 버텍스)
    DWORD iFace;
    Point3 normalVertex;
    DWORD nTextureInx;
    DWORD m_SmoothGroup, m_MtlId;

    std::vector<SkinWeightBuf *> m_SWList;

public:
    static bool CompareVertexNode( CRefVertex *first, CRefVertex *second )
    {
        return first->nId <= second->nId;
    }

};

class CXExpFrame
{
public:
    CXExpFrame(Interface *aInterface, FILE *aFile = NULL);
    ~CXExpFrame();

// Method
public:    
    LPDIRECTXFILEDATA   NodeEnum(INode* pNode, _PS ps);
    BOOL                CheckOptimize(INode* pNode);

private:    
    TriObject*  GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);
    BOOL        CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh);
    Point3      GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
    void        InitVertexFlag(int numVerts);
    int         GetVertexInx(int vertexInx);
    void        make_face_uv(Face *f, Point3 *tv);
    BOOL        GetMatID(INode *pNode, Mesh *pMesh, DWORD iFace, DWORD &MatID);
    DWORD       GetTextureIndex(INode *pNode, Mesh *pMesh, DWORD iFace, DWORD iPoint);
    BOOL        FindIdenticalVertex(DWORD iVertex, DWORD iSmooth, DWORD iMtlId, DWORD iTextureInx, DWORD &SameVert);    
    BOOL        CheckGeomObject(INode *pNode);
    
    
    HRESULT     AddTextureFilename(TCHAR *szName, _PS ps, LPDIRECTXFILEDATA pParentDataObj);
    HRESULT     AddMaterial(D3DXMATERIAL *pMaterial, _PS ps, LPDIRECTXFILEDATA pParent);
    HRESULT     GatherMeshMaterials(INode *pNode, Mesh *pMesh, DWORD **prgdwMeshMaterials, D3DXMATERIAL **prgMaterials, SCropInfo **prgCropInfo, DWORD *pcMaterials);
    TriObject*  PreProcessMesh(INode *node, TimeValue t, int &nVert, int &nFace);
    TriObject*  CreateMesh(INode* node);

    BOOL        ExportRootTransform(INode *pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj);
    BOOL        ExportGeomObject(INode *pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj, LPDIRECTXFILEDATA *ppMeshParent);
    BOOL        ExportNodeHeader(INode* pNode, TCHAR* aName);
    BOOL        ExportNodeTM(INode* node, _PS ps, LPDIRECTXFILEDATA pParentDataObj);
    BOOL        ExportMesh(INode* node, TimeValue t, _PS ps, LPDIRECTXFILEDATA pParentDataObj);
    BOOL        ExportMaterial(INode *pNode, Mesh *pMesh, DWORD *rgdwMeshMaterials, D3DXMATERIAL *rgMaterials, DWORD cMaterials, _PS ps, LPDIRECTXFILEDATA pParentDataObj);    
    BOOL        ExportVertexNormals(INode* pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj);    
    BOOL        ExportTextureCoordinates(INode* pNode, Mesh *pMesh, SCropInfo *rgCropInfo, _PS ps, LPDIRECTXFILEDATA pParentDataObj);    
    BOOL        ExportVertexDuplicationIndices(_PS ps, LPDIRECTXFILEDATA pParentDataObj);
    BOOL        ExportSkinData(INode *pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj);
// Attribute
private:
    FILE    *m_pFile;
    BOOL    m_bDeleteTriObj;

    Interface *m_pInterface;

    DWORD   m_nMaxVertexBeforeDuplication;  // (start 0)
    DWORD   m_nMaxVertex;   // (start 0)
    DWORD   m_nMaxFace;     // (start 1)

    BOOL    *m_bVertexFlag;
    BOOL    *m_bVertexCountFlag;
    DWORD   *m_VertexDuplication;

    std::vector<CRefVertex*> m_vecVertexList;
    std::vector<CFaceLIst*> m_vecFaceList;
};

#endif