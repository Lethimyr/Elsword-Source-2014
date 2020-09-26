#include "CXExpFrame.h"

//#include "XExpUtil.h"
//#include "XExpFunc.h"

#include "rmxfguid.h"
//#include "rmxftmpl.h"

#include "debug.h"
#include "DevilNet.h"

#define POWER_DEFAULT 0.0

extern char Extstr[128];
extern TextureConverter     g_TextureConverter;
extern CStringTable         g_stStrings;
extern SDialogOptions       g_DlgOptions;

CXExpFrame::CXExpFrame(Interface *aInterface, FILE *aFile)
{
    m_pInterface = aInterface;
    m_pFile = aFile;
    m_bDeleteTriObj = false;

    m_bVertexFlag = NULL;
    m_bVertexCountFlag = NULL;
    m_VertexDuplication = NULL;
}

CXExpFrame::~CXExpFrame()
{
    if(m_bVertexFlag)
        delete m_bVertexFlag;
    if(m_bVertexCountFlag)
        delete m_bVertexCountFlag;
    if(m_VertexDuplication)
        delete m_VertexDuplication;

    for(int i=0; i<m_vecVertexList.size(); ++i)
    {
        CRefVertex *temp = m_vecVertexList[i];
        if(temp)
            delete temp;
    }
    m_vecVertexList.clear();

    for(int i=0; i<m_vecFaceList.size(); ++i)
    {
        CFaceLIst *temp = m_vecFaceList[i];
        if(temp)
            delete temp;
    }
    m_vecFaceList.clear();
}

//------------------------------------------------------------------------------------------------------

TriObject* CXExpFrame::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
    deleteIt = FALSE;
    Object *obj = node->EvalWorldState(t).obj;
    obj = obj->Eval(t).obj;

    if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
    { 
        TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
        
        if (obj != tri) 
            deleteIt = TRUE;
        return tri;
    }
    else 
    {
        return NULL;
    }
}
BOOL CXExpFrame::CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh)
{
    if (!mtl || !mesh) {
        return false;
    }

    ULONG matreq = mtl->Requirements(-1);

    // Are we using face mapping?
    if (!(matreq & MTLREQ_FACEMAP)) 
    {
        return false;
    }   

    return true;
}
Point3 CXExpFrame::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
    Face* f = &mesh->faces[faceNo];
    //f->smGroup = 0;
    DWORD smGroup = f->smGroup;
    int numNormals;
    Point3 vertexNormal;

    // Is normal specified
    // SPCIFIED is not currently used, but may be used in future versions.
    if (rv->rFlags & SPECIFIED_NORMAL) 
    {
        vertexNormal = rv->rn.getNormal();
    }
    // If normal is not specified it's only available if the face belongs
    // to a smoothing group
    else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) 
    {
        // If there is only one vertex is found in the rn member.
        if (numNormals == 1 && rv->ern == NULL) 
        {
            vertexNormal = rv->rn.getNormal();            
        }
        else 
        {
            // If two or more vertices are there you need to step through them
            // and find the vertex with the same smoothing group as the current face.
            // You will find multiple normals in the ern member.
            for (int i = 0; i < numNormals; ++i) 
            {
                if (rv->ern[i].getSmGroup() & smGroup) 
                {
                    vertexNormal = rv->ern[i].getNormal();
                }
            }
        }
    }
    else 
    {
        // Get the normal from the Face if no smoothing groups are there
        vertexNormal = mesh->getFaceNormal(faceNo);    
    }
 
    vertexNormal = vertexNormal.Normalize();

    return vertexNormal;
}
void CXExpFrame::InitVertexFlag(int numVerts)
{
    for(int i=0; i<numVerts; ++i)
        m_bVertexFlag[i] = false;
}

int CXExpFrame::GetVertexInx(int vertexInx)
{
    if(m_bVertexCountFlag[vertexInx])
    {        
        ++m_nMaxVertex;
        m_VertexDuplication[vertexInx] = m_nMaxVertex;
    }
    else
    {
        m_bVertexCountFlag[vertexInx] = true;
        return vertexInx;
    }

    return m_nMaxVertex;
}

void CXExpFrame::make_face_uv(Face *f, Point3 *tv)
{
    // From the SDK
    // How to calculate UV's for face mapped materials.
    static Point3 basic_tva[3] = 
    { 
        Point3(0.0,0.0,0.0), Point3(1.0,0.0,0.0), Point3(1.0,1.0,0.0)
    };
    static Point3 basic_tvb[3] = 
    { 
        Point3(1.0,1.0,0.0), Point3(0.0,1.0,0.0), Point3(0.0,0.0,0.0)
    };
    static int nextpt[3] = {1,2,0};
    static int prevpt[3] = {2,0,1};

    int na,nhid,i;
    Point3 *basetv;
    /* make the invisible edge be 2->0 */
    nhid = 2;

    if (!(f->flags&EDGE_A))  
        nhid=0;
    else if (!(f->flags&EDGE_B)) 
        nhid = 1;
    else if (!(f->flags&EDGE_C)) 
        nhid = 2;

    na = 2-nhid;
    basetv = (f->v[prevpt[nhid]]<f->v[nhid]) ? basic_tva : basic_tvb; 
    for (i=0; i<3; i++) 
    {  
        tv[i] = basetv[na];
        na = nextpt[na];
    }
}

BOOL CXExpFrame::GetMatID(INode *pNode, Mesh *pMesh, DWORD iFace, DWORD &MatID)
{
    Mtl *pMtlMain;
    DWORD cSubMaterials;
    BOOL bNoSubMaterials;
    DWORD dwCurMatID;

    cSubMaterials = 0;

    pMtlMain = pNode->GetMtl();
    cSubMaterials = 0;
    bNoSubMaterials = FALSE;

    if (pMtlMain != NULL)
    {        
        cSubMaterials = pMtlMain->NumSubMtls();
        if (cSubMaterials < 1)
        {            
            cSubMaterials = 1;            
            bNoSubMaterials = TRUE;
        }
    }
    else  // no material, then we'll create a material corresponding to the default wire color.
    {
        //bWireframeColor = TRUE;
        cSubMaterials = 1;
    }

    if (bNoSubMaterials) 
    {        
        dwCurMatID = 0;
    }
    else
    {
        dwCurMatID = pMesh->faces[iFace].getMatID();

        if (cSubMaterials == 1)
        {
            dwCurMatID = 0;
        }
        else
        {           
            dwCurMatID %= cSubMaterials;
        }
    }     

    MatID = dwCurMatID;

    return bNoSubMaterials;
}

DWORD CXExpFrame::GetTextureIndex(INode *pNode, Mesh *pMesh, DWORD iFace, DWORD iPoint)
{
    if ((pMesh->numTVerts > 0) 
        && (NULL != pMesh->tvFace)
        && ((int)pMesh->tvFace[iFace].t[iPoint] < pMesh->numTVerts) )
    {
        //pMeshData->m_bTexCoordsPresent = TRUE;

        return pMesh->tvFace[iFace].t[iPoint];
    }
    else
        return 0xFFFFFFFF;
}


BOOL CXExpFrame::FindIdenticalVertex(DWORD iVertex, DWORD iSmooth, DWORD iMtlId, DWORD iTextureInx, DWORD &SameVert)
{
    BOOL bFound = false;
    DWORD nVertex;
    CRefVertex *VertexNode;

    if(m_bVertexFlag[iVertex] == false)       
        return false;

    nVertex = m_vecVertexList.size();
    
    for(int i=0; i<nVertex; ++i)
    {
        VertexNode = m_vecVertexList[i];

        if(VertexNode->refVertex == iVertex && VertexNode->m_SmoothGroup == iSmooth && 
            VertexNode->m_MtlId == iMtlId && VertexNode->nTextureInx == iTextureInx)
        {            
            SameVert = VertexNode->nId;
            return true;
        }
    }
    
    return false;
}

//------------------------------------------------------------------------------------------------------
HRESULT CXExpFrame::AddTextureFilename(TCHAR *szName, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr = S_OK;
    LPDIRECTXFILEDATA pDataObject = NULL;
    DWORD cbSize;

    cbSize = sizeof(TCHAR**);

    g_TextureConverter.Push_Back( szName );

    // 텍스쳐 파일경로 옵션에 따른 제지정
    if( g_DlgOptions.m_bTextureFullPath == FALSE )    
    {
        TCHAR drive[_MAX_DRIVE]	= _T("");
        TCHAR dir[_MAX_DIR]		= _T("");
        TCHAR fname[_MAX_FNAME]	= _T("");
        TCHAR ext[_MAX_EXT]		= _T("");

        _tsplitpath( szName, drive, dir, fname, ext );
        _tcscpy( szName, fname );

        _tcscpy(ext, Extstr);

        _tcscat( szName, ext ); //여기에도 확장자 명을 바꿔줘야한다.
    }
    else
    {
        TCHAR FullPath[MAX_PATH] = _T("");
        TCHAR drive[_MAX_DRIVE]	= _T("");
        TCHAR dir[_MAX_DIR]		= _T("");
        TCHAR fname[_MAX_FNAME]	= _T("");
        TCHAR ext[_MAX_EXT]		= _T("");

        _tsplitpath( szName, drive, dir, fname, ext );

        //StringCchCopy(szName,"%s%s%s%s",drive,dir,fname,".tga" );
        _tcscat(FullPath, drive ); 
        _tcscat(FullPath, dir ); 
        _tcscat(FullPath, fname ); 
        _tcscat(FullPath, Extstr );
        //이 경우에도 확장자명을 바꿔줘야한다.

        _tcscpy( szName, FullPath );
    }

    hr =ps.m_pxofsave->CreateDataObject(TID_D3DRMTextureFilename,
        NULL,
        NULL,
        cbSize,
        &szName,
        &pDataObject
        );

    if (FAILED(hr))
        goto e_Exit;

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
        goto e_Exit;

e_Exit:
    RELEASE(pDataObject);

    return hr;
}

HRESULT CXExpFrame::AddMaterial(D3DXMATERIAL *pMaterial, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr = S_OK;
    LPDIRECTXFILEDATA pDataObject = NULL;
    PBYTE pbData = NULL;
    PBYTE pbCur;
    DWORD cbSize;

    cbSize = 4*sizeof(float) // colorRGBA
        + sizeof(float) //power
        + 3*sizeof(float) //specularColor
        + 3*sizeof(float); //emissiveColor

    pbData = pbCur = new BYTE[cbSize];
    if (pbCur == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    // diffuse - write white RGBA
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Diffuse.r);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Diffuse.g);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Diffuse.b);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Diffuse.a);

    // specular power
    WRITE_FLOAT(pbCur, POWER_DEFAULT);

    // specular - write white RGBA
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Specular.r);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Specular.g);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Specular.b);

    // emmissive - write white RGBA
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Emissive.r);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Emissive.g);
    WRITE_FLOAT(pbCur, pMaterial->MatD3D.Emissive.b);

    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMMaterial,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );
    if (FAILED(hr))
        goto e_Exit;

    // if there is a bitmap texture, then add the filename
    if (pMaterial->pTextureFilename != NULL)
    {
        hr = AddTextureFilename(pMaterial->pTextureFilename, ps, pDataObject);
        if (FAILED(hr))
            goto e_Exit;
    }

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
        goto e_Exit;

e_Exit:
    delete []pbData;
    RELEASE(pDataObject);

    return hr;
}

HRESULT CXExpFrame::GatherMeshMaterials(INode *pNode, Mesh *pMesh, 
                                        DWORD **prgdwMeshMaterials, D3DXMATERIAL **prgMaterials, 
                                        SCropInfo **prgCropInfo, DWORD *pcMaterials)
{
    HRESULT hr = S_OK;
    Mtl *pMtlMain;
    Mtl *pMtlCur;
    DWORD iCurMaterial;
    DWORD cSubMaterials;
    DWORD iFace;
    DWORD cFaces;
    BOOL bNoSubMaterials;
    BOOL bWireframeColor;
    DWORD dwCurMatID;
    DWORD *rgdwMeshMaterials = NULL;
    SCropInfo *rgCropInfo = NULL;
    IParamBlock2 *pParamBlock;
    Texmap *pTexMap;
    D3DXMATERIAL *rgMaterials = NULL;
    TCHAR *szFilename;
    DWORD dwWFColor;
    D3DXCOLOR colorWF;
    BOOL bDetailMap;
    FindTextureFilename findTextureFilename;

    pMtlMain = pNode->GetMtl();
    cFaces = pMesh->getNumFaces();
    cSubMaterials = 0;
    bNoSubMaterials = FALSE;
    bWireframeColor = FALSE;

    rgdwMeshMaterials = new DWORD[cFaces];
    if (rgdwMeshMaterials == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    // The color of a given mesh can be provided by three different sources:
    //   1) applied texture maps, as part of a material
    //   2) explicitly defined & applied materials without texture maps
    //   3) a 'wireframe' color.

    // For our purposes, we will output these in this order of preference, ignoring
    // processing past the first one we find.

    if (pMtlMain != NULL)
    {
        // There is at least one material. We're in case 1) or 2)

        cSubMaterials = pMtlMain->NumSubMtls();
        if (cSubMaterials < 1)
        {
            // Count the material itself as a submaterial.
            cSubMaterials = 1;
            bNoSubMaterials = TRUE;
        }
    }
    else  // no material, then we'll create a material corresponding to the default wire color.
    {
        bWireframeColor = TRUE;
        cSubMaterials = 1;
    }

    for (iFace=0; iFace < cFaces; iFace++)
    {
        if (bWireframeColor || bNoSubMaterials) 
        {
            // If we're using wireframe color, it's our only material
            rgdwMeshMaterials[iFace] = 0;
        }
        else
        {
            // Otherwise we have at least one material to process.

            dwCurMatID = pMesh->faces[iFace].getMatID();

            if (cSubMaterials == 1)
            {
                dwCurMatID = 0;
            }
            else
            {
                // SDK recommends mod'ing the material ID by the valid # of materials, 
                // as sometimes a material number that's too high is returned.
                dwCurMatID %= cSubMaterials;
            }

            // output the appropriate material color

            rgdwMeshMaterials[iFace] = dwCurMatID;

        } 

    } 

    rgCropInfo = new SCropInfo[cSubMaterials];
    rgMaterials = new D3DXMATERIAL[cSubMaterials];
    if ((rgCropInfo == NULL) || (rgMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(rgMaterials, 0, sizeof(D3DXMATERIAL) * cSubMaterials);

    for (iCurMaterial = 0; iCurMaterial < cSubMaterials; iCurMaterial++)
    {
        rgCropInfo[iCurMaterial].fClipU = 0.0f;
        rgCropInfo[iCurMaterial].fClipV = 0.0f;
        rgCropInfo[iCurMaterial].fClipW = 1.0f;
        rgCropInfo[iCurMaterial].fClipH = 1.0f;
    }

    if (!bWireframeColor)
    {
        // 3DSMax allows multiple materials to be used on a single mesh via
        // 'submaterials'. When the first submaterial is defined, the main material
        // is copied into submaterial #1, and the new submaterial because submaterial #2.
        // 
        // We have to catch the case where there's a material, but no submaterials. For this
        // case, set NumSubMaterials to 1 which would never happen otherwise. It's imperative
        // that the first material be set to MtlMain, rather than trying to GetSubMtl() to 
        // allow this logic to work.

        // Loop through the materials (if any) and output them.

        // first collect the crop info and the list of materials
        for (iCurMaterial = 0; iCurMaterial < cSubMaterials; iCurMaterial++)
        {
            if (bNoSubMaterials)
            {
                // We're on the parent material, possibly the ONLY material.
                // We won't be able to get it with GetSubMtl() if it's the only one, and
                // the data in the first submaterial is identical to the main material,
                // so just use the main material.

                pMtlCur = pMtlMain;
            }
            else
            {
                // We're into the true submaterials. Safe to get with 'GetSubMtl'

                pMtlCur = pMtlMain->GetSubMtl(iCurMaterial);
                if(pMtlCur == NULL)
                {
                    pMtlCur = pMtlMain;                    
                }
            }

            // if using texture as a detail map, get the crop info
            bDetailMap = FALSE;
            pTexMap = pMtlCur->GetSubTexmap(ID_DI);
            if ((pTexMap != NULL) && (pTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)))
            {
                pParamBlock = pTexMap->GetParamBlock(0);

                if (pParamBlock != NULL)
                {
                    pParamBlock->GetValue(bmtex_clipu, 0, rgCropInfo[iCurMaterial].fClipU, FOREVER);
                    pParamBlock->GetValue(bmtex_clipv, 0, rgCropInfo[iCurMaterial].fClipV, FOREVER);
                    pParamBlock->GetValue(bmtex_clipw, 0, rgCropInfo[iCurMaterial].fClipW, FOREVER);
                    pParamBlock->GetValue(bmtex_cliph, 0, rgCropInfo[iCurMaterial].fClipH, FOREVER);       
                }

                bDetailMap = TRUE;
            }

            // 'Ambient color' is specified in Max docs as "the color of the object in shadows," and 
            //                 is usually a darker version of the diffuse color.
            // 'Diffuse color' is specified in the Max docs as "the color of the object in 
            //                  good lighting," usually referred to as the objects' color.
            // 'Specular color' is specified as the color of reflection highlights in direct lighting,
            //                  and according to Max docs is usually the same as diffuse.

            if (!bDetailMap)
            {
                // We're going to use the 'Diffuse' color as the object color for DirectX
                rgMaterials[iCurMaterial].MatD3D.Diffuse.r = pMtlCur->GetDiffuse().r;
                rgMaterials[iCurMaterial].MatD3D.Diffuse.g = pMtlCur->GetDiffuse().g;
                rgMaterials[iCurMaterial].MatD3D.Diffuse.b = pMtlCur->GetDiffuse().b;

                //Alpha
                rgMaterials[iCurMaterial].MatD3D.Diffuse.a = 1.0f - pMtlCur->GetXParency();

                // 3DSMax specular power is comprised of shininess, and shininess strength.
                // TODO - figure out a mapping from shininess to power
                rgMaterials[iCurMaterial].MatD3D.Power = POWER_DEFAULT;

                // Specular
                rgMaterials[iCurMaterial].MatD3D.Specular.r = pMtlCur->GetSpecular().r;
                rgMaterials[iCurMaterial].MatD3D.Specular.g = pMtlCur->GetSpecular().g;
                rgMaterials[iCurMaterial].MatD3D.Specular.b = pMtlCur->GetSpecular().b;
                rgMaterials[iCurMaterial].MatD3D.Specular.a = 1.0f;

                // Emmissive
                rgMaterials[iCurMaterial].MatD3D.Emissive.r = pMtlCur->GetSelfIllumColor().r;
                rgMaterials[iCurMaterial].MatD3D.Emissive.g = pMtlCur->GetSelfIllumColor().g;
                rgMaterials[iCurMaterial].MatD3D.Emissive.b = pMtlCur->GetSelfIllumColor().b;
                rgMaterials[iCurMaterial].MatD3D.Emissive.a = 1.0f;
            }
            else  // if a detail map, then don't write the color
            {
                // diffuse - write white RGBA
                rgMaterials[iCurMaterial].MatD3D.Diffuse.r = 1.0f;
                rgMaterials[iCurMaterial].MatD3D.Diffuse.g = 1.0f;
                rgMaterials[iCurMaterial].MatD3D.Diffuse.b = 1.0f;
                rgMaterials[iCurMaterial].MatD3D.Diffuse.a = 1.0f;

                rgMaterials[iCurMaterial].MatD3D.Power = POWER_DEFAULT;

                // specular - write white RGBA
                rgMaterials[iCurMaterial].MatD3D.Specular.r = 1.0f;
                rgMaterials[iCurMaterial].MatD3D.Specular.g = 1.0f;
                rgMaterials[iCurMaterial].MatD3D.Specular.b = 1.0f;
                rgMaterials[iCurMaterial].MatD3D.Specular.a = 1.0f;

                // emmissive - write white RGBA
                rgMaterials[iCurMaterial].MatD3D.Emissive.r = 0.0f;
                rgMaterials[iCurMaterial].MatD3D.Emissive.g = 0.0f;
                rgMaterials[iCurMaterial].MatD3D.Emissive.b = 0.0f;
                rgMaterials[iCurMaterial].MatD3D.Emissive.a = 1.0f;
            }

            if (bDetailMap)
            {
                pTexMap->EnumAuxFiles(findTextureFilename, FILE_ENUM_ALL);

                if (findTextureFilename.m_szTextureName == NULL)
                {
                    //XSkinExp_OutputDebugString("AddMaterial: Bitmap texture found, but no texture name\n");
                    szFilename = NULL;
                }
                else // texture filename found
                {
                    // allocate a new string, doing the '\' fixup if neccessary
                    if (g_DlgOptions.m_xFormat == DXFILEFORMAT_TEXT)
                        szFilename = g_stStrings.CreateNiceFilename(findTextureFilename.m_szTextureName); /*expand \ char to \\ */
                    else
                        szFilename = g_stStrings.CreateNormalFilename(findTextureFilename.m_szTextureName); /*DON'T! expand \ char to \\ */

                    if (szFilename == NULL)
                    {
                        hr = E_OUTOFMEMORY;
                        goto e_Exit;
                    }
                }

                rgMaterials[iCurMaterial].pTextureFilename = szFilename;
            }
        } 
    }
    else // wireframe color
    {
        // get the wireframe color
        dwWFColor = pNode->GetWireColor();
        //###danhoro $REMOVE no need for this
        //dwWFColor |= 0xff000000;  // set alpha to fully opaque

        // convert to floating point
        //###danhoro $REMOVE this was wrong and inverted the colors
        //colorWF = D3DXCOLOR(dwWFColor);

        //###danhoro $NOTE added to convert COLORREF to D3DXCOLOR
        //XBGR: 0x00bbggrr
        colorWF.r= ((dwWFColor >>  0) & 0xff) / 256.0f;
        colorWF.g= ((dwWFColor >>  8) & 0xff) / 256.0f;
        colorWF.b= ((dwWFColor >> 16) & 0xff) / 256.0f;
        colorWF.a= 1.0f;

        //RGBA
        rgMaterials[0].MatD3D.Diffuse.r = colorWF.r;
        rgMaterials[0].MatD3D.Diffuse.g = colorWF.g;
        rgMaterials[0].MatD3D.Diffuse.b = colorWF.b;
        rgMaterials[0].MatD3D.Diffuse.a = 1.0f;

        // Wireframe doesn't have an explicit specular power, so output our default.
        rgMaterials[0].MatD3D.Power = POWER_DEFAULT;

        // Set the specular color identical to diffuse, as recommended in 3DSMax docs.
        rgMaterials[0].MatD3D.Specular.r = colorWF.r;
        rgMaterials[0].MatD3D.Specular.g = colorWF.g;
        rgMaterials[0].MatD3D.Specular.b = colorWF.b;
        rgMaterials[0].MatD3D.Specular.a = 1.0f;

        // Set the luminence to 0: the material isn't glowing.
        rgMaterials[0].MatD3D.Emissive.r = 0.0f;
        rgMaterials[0].MatD3D.Emissive.g = 0.0f;
        rgMaterials[0].MatD3D.Emissive.b = 0.0f;
        rgMaterials[0].MatD3D.Emissive.a = 1.0f;

    }

    *prgdwMeshMaterials = rgdwMeshMaterials;
    rgdwMeshMaterials = NULL;

    *prgCropInfo = rgCropInfo;
    rgCropInfo = NULL;

    *prgMaterials = rgMaterials;
    rgMaterials = NULL;

    *pcMaterials = cSubMaterials;

e_Exit:
    delete []rgdwMeshMaterials;
    delete []rgMaterials;
    delete []rgCropInfo;
    return hr;
}


TriObject* CXExpFrame::PreProcessMesh(INode *node, TimeValue t, int &nVert, int &nFace)
{
    //CRefVertex *vertexNode;
    CFaceLIst *faceNode;
    BOOL bGroup;    
    int vx1, vx2, vx3;
    int numVerts, numFaces;

    TriObject* tri = GetTriObjectFromNode(node, t, m_bDeleteTriObj);
    if (!tri) 
    {
        return NULL;
    }

    Mesh* mesh = &tri->GetMesh();    

    if(g_DlgOptions.m_bSmoothNormal)
        mesh->AutoSmooth(PI, FALSE);
    

    if(g_DlgOptions.m_bOptimize)
    {
        mesh->RemoveIllegalFaces();
        mesh->RemoveDegenerateFaces();
        mesh->DeleteIsoVerts();
        mesh->DeleteIsoMapVerts();

        mesh->checkNormals(TRUE);        
    }
    else
    {
        mesh->buildNormals();           
    }    
    
    //mesh->UnifyNormals(FALSE);
    //mesh->buildNormals();   
    

    // 오브젝트 상태 획득
    const ObjectState ObjState = node->EvalWorldState(0);
    Object* pObj = ObjState.obj;   

    
#if 1
    numVerts = mesh->getNumVerts();    
    numFaces = mesh->getNumFaces();
#else
    GetTriMeshFaceCount(0, pObj, numFaces, numVerts);
#endif

    nVert = numVerts;
    nFace = numFaces;

    m_nMaxVertexBeforeDuplication = numVerts-1;

    m_bVertexFlag = new BOOL[numVerts];
    m_bVertexCountFlag = new BOOL[numVerts];
    m_VertexDuplication = new DWORD[numVerts];

    InitVertexFlag(numVerts);
    for(int i=0; i<numVerts; ++i)
    {
        m_bVertexCountFlag[i] = false;
        m_VertexDuplication[i] = i;
    }

    m_nMaxVertex = numVerts-1;

    // CCW
    vx1 = 0;
    vx2 = 1;
    vx3 = 2;


    // 기본 페이스 정보 저장
    for (int i=0; i<numFaces; ++i) 
    {
        DWORD MatId;

        GetMatID(node, mesh, i, MatId);

        faceNode = new CFaceLIst();
        faceNode->m_nId = i;
        faceNode->pFace = &mesh->faces[i];
        faceNode->m_VertexInx[0] = mesh->faces[i].v[vx1];
        faceNode->m_VertexInx[1] = mesh->faces[i].v[vx2];
        faceNode->m_VertexInx[2] = mesh->faces[i].v[vx3];
        faceNode->m_SmoothGroup = mesh->faces[i].getSmGroup();
        faceNode->m_MtlId = MatId;        

        bGroup = false;

        m_vecFaceList.push_back(faceNode);
    }    

    return tri;
}

BOOL CXExpFrame::ExportRootTransform(INode *pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr;
    LPDIRECTXFILEDATA pDataObject = NULL;
    Matrix3 matNodeTM;
    Matrix3 matRelativeTM;   
    PBYTE pbData = NULL;
    PBYTE pbCur;
    DWORD cbSize;

    cbSize = 16*sizeof(float);
    pbCur = pbData = new BYTE[cbSize];
    if (pbData == NULL)
    {        
        delete []pbData;
        RELEASE(pDataObject);
        return false;
    }

    // FrameTransformMatrix 생성
    // ToDo

    // Node transform
    pNode->EvalWorldState(0);
    matNodeTM = pNode->GetNodeTM(0);

    if( pNode->IsRootNode() )
    {
        // scene root
        matRelativeTM = matNodeTM;
    }
    else
    {
        INode *pParentNode= pNode->GetParentNode();

        if(pParentNode == NULL)
        {
            MessageBox(NULL, "Scen Root Transform Export error!", "Error", MB_OK);
            return false;
        }

        pParentNode->EvalWorldState(0);
        Matrix3 matParentTM= pParentNode->GetNodeTM(0);

        if( matNodeTM == matParentTM )
        {
            matRelativeTM.IdentityMatrix();
        }
        else
        {
            matRelativeTM = matNodeTM * Inverse(matParentTM);
        }
    }

    WRITE_MATRIX4_FROM_MATRIX3(pbCur, matRelativeTM);

    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMFrameTransformMatrix,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );
    if (FAILED(hr))
    {
        delete []pbData;
        RELEASE(pDataObject);
        return false;
    }

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
    {
        delete []pbData;
        RELEASE(pDataObject);
        return false;
    }

    return true;
}

BOOL CXExpFrame::CheckGeomObject(INode *pNode)
{
    BOOL bOptimize, result;
    int nOptimizeVertex, nNOptimizeVertex, nOptimizeFace, nNOptimizeFace;

    TriObject* triOptimize = GetTriObjectFromNode(pNode, 0, bOptimize);
    
    result = true;

    if (!triOptimize) 
    {
        return false;
    }

    Mesh* meshOptimize = &triOptimize->GetMesh(); 


    meshOptimize->RemoveIllegalFaces();
    meshOptimize->RemoveDegenerateFaces();
    meshOptimize->DeleteIsoVerts();
    meshOptimize->DeleteIsoMapVerts();
    meshOptimize->checkNormals(TRUE);

    // 오브젝트 상태 획득
    const ObjectState ObjState = pNode->EvalWorldState(0);
    Object* pObj = ObjState.obj;   


    nOptimizeVertex = meshOptimize->getNumVerts();    
    nOptimizeFace = meshOptimize->getNumFaces();

    GetTriMeshFaceCount(0, pObj, nNOptimizeFace, nNOptimizeVertex);


    g_DlgOptions.m_nTotalVertex1 += nOptimizeVertex;
    g_DlgOptions.m_nTotalVertex2 += nNOptimizeVertex;
    g_DlgOptions.m_nTotalFace1 += nOptimizeFace;
    g_DlgOptions.m_nTotalFace2 += nNOptimizeFace;

    if(nOptimizeVertex != nNOptimizeVertex || nOptimizeFace != nNOptimizeFace)
        result = false;

    if(bOptimize)
    {
        if (triOptimize) 
            delete triOptimize;
        bOptimize = false;
    }

    return result;
}

BOOL CXExpFrame::CheckOptimize(INode *pNode)
{
    BOOL result = true;

    ObjectState os = pNode->EvalWorldState(0); 

    // The obj member of ObjectState is the actual object we will export.
    if (os.obj) 
    {
        // We look at the super class ID to determine the type of the object.
        switch(os.obj->SuperClassID()) {
            case GEOMOBJECT_CLASS_ID: 
                result = CheckGeomObject(pNode); 
                break;
        }
    }   

    ++g_DlgOptions.m_nTotalNode;

    if(!result)
    {
        g_DlgOptions.m_bOptimize = false;        
    }

    // For each child of this node, we recurse into ourselves 
    // until no more children are found.
    CXExpFrame *pFrame;

    for (int i = 0; i < pNode->NumberOfChildren(); ++i) 
    {
        pFrame = new CXExpFrame(m_pInterface, m_pFile);

        pFrame->CheckOptimize(pNode->GetChildNode(i));

        delete pFrame;
    }

    return true;
}


LPDIRECTXFILEDATA CXExpFrame::NodeEnum(INode* pNode, _PS ps)
{   
    char *szName = NULL;
    BOOL result = true;
    HRESULT hr;
    LPDIRECTXFILEDATA pDataObject = NULL;
    LPDIRECTXFILEDATA pMeshParent = NULL;
    LPDIRECTXFILEDATA pMeshOriParent = NULL;

    // Stop recursing if the user pressed Cancel 
    if (m_pInterface->GetCancel())
        return NULL;    

    // GROUP
    if (pNode->IsGroupHead()) 
    {
        // 그룹 처리
    }        

    ps.m_pSkinData->PushNode(pNode); 

    if ( 1 ) //!(g_DlgOptions.m_bSelection && !pNode->Selected()) )
    {        
        szName = g_stStrings.CreateNiceString(pNode->GetName());
        if(szName != NULL)
        {
            hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMFrame,
                szName,
                NULL,
                0,
                NULL,
                &pDataObject
                );
            if(FAILED(hr))
                return NULL;

            ExportRootTransform(pNode, ps, pDataObject);

            pMeshParent = pDataObject;
        }       


        ObjectState os = pNode->EvalWorldState(0); 
        

        // The obj member of ObjectState is the actual object we will export.
        if (os.obj) 
        {
#if 1 
            // We look at the super class ID to determine the type of the object.
            switch(os.obj->SuperClassID()) {
            case GEOMOBJECT_CLASS_ID: 
                result = ExportGeomObject(pNode, ps, pDataObject, &pMeshParent); 
                break;
            case CAMERA_CLASS_ID:
                //if (GetIncludeObjCamera()) ExportCameraObject(node, indentLevel);                 
                break;
            case LIGHT_CLASS_ID:
                //if (GetIncludeObjLight()) ExportLightObject(node, indentLevel);                 
                break;
            case SHAPE_CLASS_ID:
                //if (GetIncludeObjShape()) ExportShapeObject(node, indentLevel);                 
                break;
            case HELPER_CLASS_ID:
                //if (GetIncludeObjHelper()) ExportHelperObject(node, indentLevel);                 
                break;
            }
#else
            result = ExportGeomObject(pNode, ps, pDataObject, &pMeshParent); 
#endif
        }

        if(!result)
        {
            //MessageBox(NULL, "ExportGeomObject Fail!", "Error!", MB_OK);
            return NULL;
        }
    }
    else
    {
        pDataObject = NULL;
        pMeshParent = NULL;
    }

    // 진행률 표시
    int iProcess;    
    ++g_DlgOptions.m_iCurNode;
    iProcess = (((float)g_DlgOptions.m_iCurNode / (float)g_DlgOptions.m_nTotalNode) * 40.0f) + 10;
    m_pInterface->ProgressUpdate(iProcess);

    // For each child of this node, we recurse into ourselves 
    // until no more children are found.
    CXExpFrame *pFrame;

    for (int i = 0; i < pNode->NumberOfChildren(); ++i) 
    {
        LPDIRECTXFILEDATA pChildDataObject;

        pFrame = new CXExpFrame(m_pInterface, m_pFile);

        pChildDataObject = pFrame->NodeEnum(pNode->GetChildNode(i), ps);
        if(pChildDataObject)
        {
            if(!pMeshParent)
            {
                szName = g_stStrings.CreateNiceString(pNode->GetName());
                if(szName != NULL)
                {
                    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMFrame,
                        szName,
                        NULL,
                        0,
                        NULL,
                        &pDataObject
                        );
                    if(FAILED(hr))
                        return NULL;

                    ExportRootTransform(pNode, ps, pDataObject);                    
                }
                else
                {
                    delete pFrame;
                    return NULL;
                }
            }
            
            pMeshOriParent = pDataObject;
            

            hr = pMeshOriParent->AddDataObject(pChildDataObject);
            if(FAILED(hr))
            {
                delete pFrame;
                return NULL;
            }
        }
        
        
        delete pFrame;
    }

    
    if (pNode->IsGroupHead()) 
    {
        // 그룹종료
    }

    return pDataObject;
}


BOOL CXExpFrame::ExportGeomObject(INode *pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj, LPDIRECTXFILEDATA *ppMeshParent)
{
    HRESULT hr;
    PBYTE pbData = NULL;
    PBYTE pbCur;
    DWORD cbSize;
    Object* pObj;
    Matrix3 matNodeTM;
    Matrix3 matObjTMAfterWSM;
    Matrix3 matObjectOffset;

    LPDIRECTXFILEDATA pDataObject = NULL;
    LPDIRECTXFILEDATA pObjectOffset = NULL;

    ObjectState os = pNode->EvalWorldState(0);
    if (!os.obj)
        return false;

    if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
        return false;

    
    if( g_DlgOptions.m_bChildrenNotAttached == TRUE )
        matNodeTM = pNode->GetObjectTM(0);	//offset이 적용된 tm
    else
        matNodeTM = pNode->GetNodeTM(0);	//offset이 적용되지 않은 tm

    matObjTMAfterWSM = pNode->GetObjTMAfterWSM(0);

    if( IsExportableMesh(pNode, &pObj) && !(g_DlgOptions.m_bSelection && !pNode->Selected()))
    {
        if( !(matObjTMAfterWSM == matNodeTM) )
        {
            hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMFrame,
                NULL,
                NULL,
                0,
                NULL,
                &pObjectOffset
                );

            matObjectOffset = matObjTMAfterWSM * Inverse(matNodeTM);        
            //matObjectOffset =  matNodeTM * Inverse(matObjTMAfterWSM);        

            cbSize = 16*sizeof(float);

            pbCur = pbData = new BYTE[cbSize];
            WRITE_MATRIX4_FROM_MATRIX3(pbCur, matObjectOffset);

            hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMFrameTransformMatrix,
                NULL,
                NULL,
                cbSize,
                pbData,
                &pDataObject
                );

            hr = pObjectOffset->AddDataObject(pDataObject);
            hr = pParentDataObj->AddDataObject(pObjectOffset);

            *ppMeshParent = pObjectOffset;
        }        

        // 메쉬추출
        return ExportMesh(pNode, 0, ps, *ppMeshParent);
    }
    else
    {
        *ppMeshParent = pParentDataObj;
    }    

    // 메쉬추출 (여기 있으면 본이고 뭐고 다 가져온다.)    
    //ExportMesh(pNode, 0, ps, *ppMeshParent);

    return true;
}

BOOL CXExpFrame::ExportNodeHeader(INode* pNode, TCHAR* aName)
{
    TCHAR *frameName = NULL;

    if(aName == NULL)
    {
        if(pNode == NULL)
            return false;
        frameName = pNode->GetName();
    }
    else
    {
        frameName = aName;
    }

    return true;
}

BOOL CXExpFrame::ExportNodeTM(INode* pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{    
    HRESULT hr = S_OK;
    Matrix3 matNodeTM;
    Matrix3 matObjTMAfterWSM;
    Matrix3 matObjectOffset;
    PBYTE pbData = NULL;
    PBYTE pbCur;
    DWORD cbSize;
    LPDIRECTXFILEDATA pDataObject = NULL;

    cbSize = 16*sizeof(float);

    pbCur = pbData = new BYTE[cbSize];
    if (pbData == NULL)
    {
        delete []pbData;
        RELEASE(pDataObject);
        return false;
    }

    
    //matNodeTM = pNode->GetObjectTM(0);	        //offset이 적용된 tm (offset?????????)
    matNodeTM = pNode->GetNodeTM(0);	            //offset(?)이 적용되지 않은 tm

    matObjTMAfterWSM = pNode->GetObjTMAfterWSM(0);  // world matrix

    if( matObjTMAfterWSM == matNodeTM )
    {
        matObjectOffset = pNode->GetParentTM(0);

    }
    else  // identity object offset, mesh should use node as parent
    {
        matObjectOffset = matObjTMAfterWSM * Inverse(matNodeTM);
    }

    WRITE_MATRIX4_FROM_MATRIX3(pbCur, matObjectOffset);

    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMFrameTransformMatrix,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );
    if(FAILED(hr))
    {
        delete []pbData;
        RELEASE(pDataObject);
        return false;
    }

    hr = pParentDataObj->AddDataObject(pDataObject);
    if(FAILED(hr))
    {
        delete []pbData;
        RELEASE(pDataObject);
        return false;
    }

    return true;
}

TriObject* CXExpFrame::CreateMesh(INode* node)
{
    TimeValue t = 0;
    int i, numVerts, numFaces;
    Mtl* nodeMtl = node->GetMtl();
    Matrix3 tm = node->GetObjTMAfterWSM(t);    
    BOOL bGroup = false;
    int vx1, vx2, vx3;
    TSTR indent;
    TriObject *tri;   
    Mesh* mesh;

    // DX 좌표계
    vx1 = 0;
    vx2 = 2;
    vx3 = 1;

    ObjectState os = node->EvalWorldState(t);
    if (!os.obj || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID) 
    {
        return NULL; // Safety net. This shouldn't happen.
    }


    tri = PreProcessMesh(node, t, numVerts, numFaces);   
    if(tri == NULL || numFaces <= 0)
        return NULL;

    mesh = &tri->GetMesh();    
    m_nMaxFace = m_vecFaceList.size();

    // SmoothGroup 통합
    //m_vecFaceList[0]->m_SmoothGroup = 0;
    //Face* f = &mesh->faces[0];
    //f->smGroup = 0;

    DWORD nSmoothGroup = m_vecFaceList[0]->m_SmoothGroup;
    DWORD nMtlId = m_vecFaceList[0]->m_MtlId;  
    DWORD nTextureInx = 0xFFFFFFFF;

    for(i=0; i<m_nMaxFace; ++i)
    {
        CFaceLIst *faceNode = m_vecFaceList[i];

        // SmoothGroup 통합
        //faceNode->m_SmoothGroup = 0;

        int   nId = faceNode->m_nId;
        DWORD tempSmoothGroup = faceNode->m_SmoothGroup;
        DWORD tempMtlId = faceNode->m_MtlId;                


        for(int j=0; j<3; ++j)
        {
            DWORD SameVert = 0;
            DWORD vertexInx = faceNode->m_VertexInx[j];
            DWORD tempTextureInx = GetTextureIndex(node, mesh, i, j);
            BOOL  bFound = FindIdenticalVertex(vertexInx, tempSmoothGroup, tempMtlId, tempTextureInx, SameVert);


            if(!bFound)
            {
                // 등록된 버텍스가 아닌경우
                CRefVertex *vertexNode = new CRefVertex();

                vertexNode->nId = GetVertexInx(vertexInx);                
                vertexNode->refVertex = vertexInx;
                vertexNode->iFace = i;
                vertexNode->nTextureInx = tempTextureInx;
                vertexNode->m_SmoothGroup = tempSmoothGroup;
                vertexNode->m_MtlId = tempMtlId;
                
                m_vecVertexList.push_back((vertexNode));

                m_bVertexFlag[vertexInx] = true;  
                faceNode->m_VertexInx[j] = vertexNode->nId; 

                // 노멀값
                Point3 vn;  // Vertex normal
                int  vert;
                vert = vertexNode->refVertex; // faceNode->pFace->getVert(j);
                vn = GetVertexNormal(mesh, faceNode->m_nId, mesh->getRVertPtr(vert));                
                //ConvertDXPoision(vn);
                vertexNode->normalVertex = vn;   
            }
            else
            {
                faceNode->m_VertexInx[j] = SameVert; //m_VertexDuplication[vertexInx];
            }               
        }    
    }

    BOOL *ArrangeVertex = new BOOL[numVerts];

        
    for(int i=0; i<numVerts; ++i)
    {
        if(m_bVertexCountFlag[i] == FALSE)
        {
            CRefVertex *vertexNode = new CRefVertex();

            vertexNode->nId = i;                
            vertexNode->refVertex = i;
            vertexNode->iFace = 0;
            vertexNode->nTextureInx = 0xFFFFFFFF;
            vertexNode->m_SmoothGroup = 0;
            vertexNode->m_MtlId = 0;           
            vertexNode->normalVertex = Point3(0.0f, 0.0f, 0.0f) ;

            m_vecVertexList.push_back(vertexNode);
        }
    }

    sort(m_vecVertexList.begin(), m_vecVertexList.end(), CRefVertex::CompareVertexNode);
    return tri;
}

BOOL CXExpFrame::ExportMesh(INode* pNode, TimeValue t, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr;
    int v0, v1, v2;
    BOOL    bSwapTriOrder = false;
    PBYTE   pbData = NULL;
    PBYTE   pbCur;
    DWORD   cbSize;
    DWORD   cVertices, cFaces;
    DWORD   iFace;
    Point3  vert;
    Matrix3 matNodeTM;

    DWORD *rgdwMeshMaterials = NULL;
    D3DXMATERIAL *rgMaterials = NULL;
    SCropInfo *rgCropInfo = NULL;
    DWORD cMaterials;

    TriObject *tri;
    Mesh *mesh;
    CRefVertex *vertexNode = NULL;
    CFaceLIst *faceNode = NULL;

    LPDIRECTXFILEDATA pDataObject = NULL;   
    
        
            
    tri = CreateMesh(pNode);
    if(tri == NULL)
    {
        cVertices = 0;
        cFaces = 0;
        return false;
    }
    else
    {
        mesh = &tri->GetMesh();
        hr = GatherMeshMaterials(pNode, mesh, &rgdwMeshMaterials, &rgMaterials, &rgCropInfo, &cMaterials);
    }
    

    matNodeTM = pNode->GetObjTMAfterWSM(0);
    bSwapTriOrder = !matNodeTM.Parity();

    if(bSwapTriOrder)
    {
        v0 = 0;
        v1 = 2;
        v2 = 1;
    }
    else
    {
        v0 = 0;
        v1 = 1;
        v2 = 2;
    }

    if(tri != NULL)
    {
        cVertices = m_nMaxVertex + 1;
        cFaces = m_nMaxFace;
    }
    
    cbSize = sizeof(DWORD) // nVertices
        + cVertices*sizeof(float)*3 // vertices
        + sizeof(DWORD) // nFaces
        + cFaces*(sizeof(DWORD) /*nFaceVertexIndices*/ 
        + sizeof(DWORD)*3 /*faceVertexIndices*/); // faces

    pbCur = pbData = new BYTE[cbSize];
    if (pbData == NULL)
    {        
        if(m_bDeleteTriObj)
        {
            if (tri) 
                delete tri;
            m_bDeleteTriObj = false;
        }

        return false;
    }

    // write nVertices
    WRITE_DWORD(pbCur, cVertices);

    // write vertices
    for (int i = 0; i < cVertices; ++i)
    {
        
        vertexNode = m_vecVertexList[i];        
        vert = mesh->getVert(vertexNode->refVertex);
        //vert = mesh->verts[vertexNode->refVertex];    

        if(vertexNode != NULL)
        {
#if 0
            Matrix3 wMat1, wMat2;
            Point3 localVertex, worldVertex;

            pNode->EvalWorldState(0);
            wMat1 = pNode->GetObjTMAfterWSM(0);
            wMat2 = pNode->GetNodeTM(0);

            worldVertex = wMat1 * vert;
            localVertex = worldVertex * Inverse(wMat2);


            WRITE_POINT3(pbCur, localVertex);

#else
            WRITE_POINT3(pbCur, vert);
#endif 
        }       
        vertexNode = NULL;                  
    }

    // write nFaces
    WRITE_DWORD(pbCur, m_nMaxFace);

    // write faces
    for(int i = 0; i < cFaces; ++i)
    {
        iFace = 0;

        while(iFace < cFaces)
        {
            faceNode = m_vecFaceList[iFace];
            if(faceNode->m_nId == i)
                break;

            ++iFace;
        }

        if(faceNode != NULL)
        {
            WRITE_DWORD(pbCur, 3); //nFaceVertexIndices

            // face vertex indices                          
            WRITE_DWORD(pbCur, faceNode->m_VertexInx[v0]);
            WRITE_DWORD(pbCur, faceNode->m_VertexInx[v1]);
            WRITE_DWORD(pbCur, faceNode->m_VertexInx[v2]);
            
            faceNode = NULL;
        }        
    }

    // write Mesh
    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMMesh,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );

    if (FAILED(hr))
    {        
        RELEASE(pDataObject);
        if(m_bDeleteTriObj)
        {
            if (tri) 
                delete tri;
            m_bDeleteTriObj = false;
        }

        return false;
    }


    if(tri != NULL)
    {
        // write Vertex Normals
        ExportVertexNormals(pNode, ps, pDataObject);

        // write texture coordinates
        ExportTextureCoordinates(pNode, mesh, rgCropInfo, ps, pDataObject);

        // write VertexDuplicationIndices    
        ExportVertexDuplicationIndices(ps, pDataObject);

        // write Materials    
        ExportMaterial(pNode, mesh, rgdwMeshMaterials, rgMaterials, cMaterials, ps, pDataObject);

        // write Skin Data    
        ExportSkinData(pNode, ps, pDataObject);
    }    

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
    {        
        RELEASE(pDataObject);
        if(m_bDeleteTriObj)
        {
            if (tri) 
                delete tri;
            m_bDeleteTriObj = false;
        }

        return false;
    }

    RELEASE(pDataObject);
    if(m_bDeleteTriObj)
    {
        if (tri) 
            delete tri;
        m_bDeleteTriObj = false;
    }

    return true;
}


BOOL CXExpFrame::ExportMaterial(INode *pNode, Mesh *pMesh, DWORD *rgdwMeshMaterials, D3DXMATERIAL *rgMaterials,
                                DWORD cMaterials, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr = S_OK;
    LPDIRECTXFILEDATA pDataObject = NULL;
    DWORD iFace;
    DWORD cFaces;
    PBYTE pbCur;
    PBYTE pbData = NULL;
    DWORD cbSize;
    DWORD iCurMaterial;
    BOOL bFound;
    DWORD cUniqueMaterials;
    DWORD iSearchMaterial;
    DWORD iPrevMaterial;
    DWORD iDestMaterial;
    DWORD *rgdwMaterialRemap = NULL;

    cFaces = pMesh->getNumFaces();


    rgdwMaterialRemap = new DWORD[cMaterials];
    if (rgdwMaterialRemap == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    // first go through and remove redundant materials (might not have had the same crop info!)
    cUniqueMaterials = 1;
    rgdwMaterialRemap[0] = 0;
    for (iCurMaterial = 1; iCurMaterial < cMaterials; iCurMaterial++)
    {
        bFound = FALSE;
        for (iSearchMaterial = 0; iSearchMaterial < iCurMaterial; iSearchMaterial++)
        {
            if (iSearchMaterial == iCurMaterial)
                continue;

            // if both the material and texture name are the same, then it is a redundant material
            if ((memcmp(&(rgMaterials[iCurMaterial].MatD3D), &(rgMaterials[iSearchMaterial].MatD3D), sizeof(D3DMATERIAL9)) == 0)
                && ( (rgMaterials[iCurMaterial].pTextureFilename == rgMaterials[iSearchMaterial].pTextureFilename)
                || ((rgMaterials[iCurMaterial].pTextureFilename != NULL) 
                && (rgMaterials[iSearchMaterial].pTextureFilename != NULL)
                && (strcmp(rgMaterials[iCurMaterial].pTextureFilename, rgMaterials[iSearchMaterial].pTextureFilename) == 0))))
            {
                bFound = TRUE;
                break;
            }

        }

        // if found, just remap to the material that was found
        if (bFound)
        {
            rgdwMaterialRemap[iCurMaterial] = rgdwMaterialRemap[iSearchMaterial];
        }
        else  // not found, another unique material
        {
            rgdwMaterialRemap[iCurMaterial] = cUniqueMaterials;
            cUniqueMaterials += 1;
        }
    }

    // now remap the materials
    iPrevMaterial = 0;
    for (iCurMaterial = 1; iCurMaterial < cMaterials; iCurMaterial++)
    {
        iDestMaterial = rgdwMaterialRemap[iCurMaterial];

        // if a unique material, then move it
        if (iDestMaterial > iPrevMaterial)
        {
            iPrevMaterial += 1;
            assert(iDestMaterial == iPrevMaterial);

            // if not staying in the same place, then copy it
            if (iCurMaterial != iDestMaterial)
            {
                memcpy(&rgMaterials[iDestMaterial], &rgMaterials[iCurMaterial], sizeof(D3DXMATERIAL));
            }
        }
    }


    cbSize = sizeof(DWORD) // nMaterials
        + sizeof(DWORD) // nFaceIndexes
        + cFaces*sizeof(DWORD); // face indexes

    pbData = pbCur = new BYTE[cbSize];
    if (pbCur == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    WRITE_DWORD(pbCur, cUniqueMaterials);
    WRITE_DWORD(pbCur, cFaces);


    // For each face, output the index of the material which applies to it, 
    // starting from  0

    for (iFace=0; iFace < cFaces; iFace++)
    {
        // don't forget to remap the matrerial before writing it to the file
        WRITE_DWORD(pbCur, rgdwMaterialRemap[rgdwMeshMaterials[iFace]];);
    } 

    // now finally create the mesh material list
    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMMeshMaterialList,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );
    if (FAILED(hr))
        goto e_Exit;

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
        goto e_Exit;

    for (iCurMaterial = 0; iCurMaterial < cUniqueMaterials; iCurMaterial++)
    {
        hr = AddMaterial(&rgMaterials[iCurMaterial], ps, pDataObject);
        if (FAILED(hr))
            goto e_Exit;
    } 

e_Exit:
    delete []pbData;
    delete []rgdwMaterialRemap;
    RELEASE(pDataObject);


    return hr;
}

BOOL CXExpFrame::ExportVertexNormals(INode* pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr;
    PBYTE   pbData = NULL;
    PBYTE   pbCur;
    DWORD   cbSize;
    DWORD   cNormals, cFaces;
    DWORD   iFace;
    Point3  vert;
    BOOL    bSwapTriOrder;
    Matrix3 matNodeTM;

    CRefVertex *vertexNode = NULL;
    CFaceLIst *faceNode = NULL;

    LPDIRECTXFILEDATA pDataObject = NULL;

    matNodeTM = pNode->GetObjTMAfterWSM(0);
    bSwapTriOrder = !matNodeTM.Parity();

    cNormals = m_nMaxVertex + 1;
    cFaces = m_nMaxFace;
    cbSize = sizeof(DWORD) // nNormals
        + 3*sizeof(float)*cNormals // normals
        + sizeof(DWORD) // nFaces
        + cFaces* // MeshFace array
        (sizeof(DWORD) //nFaceVertexIndices (number of normal indices)
        + 3*sizeof(DWORD)); // faceVertexIndices (normal indices)

    pbCur = pbData = new BYTE[cbSize];
    if (pbData == NULL)
        return false;

#if 1 
    if(g_DlgOptions.m_bSmoothNormal)
    {
        // Calc Smooth Normal
        DWORD cntNVert = 0;
        BOOL bCalcNormal = TRUE;
        CRefVertex *vertexNode1 = NULL;
        Point3  normalVert, tempVec;
        std::vector<Point3> vecCheckVect;

        for (int i = 0; i < cNormals; ++i)
        {            
            vertexNode = m_vecVertexList[i];

            if(vertexNode != NULL)
            {
                vert = vertexNode->normalVertex;

                if(vertexNode->refVertex == vertexNode->nId)
                {
                    cntNVert = 0;
                    bCalcNormal = TRUE;
                    vecCheckVect.clear();
                    normalVert = Point3(0.0f, 0.0f, 0.0f);
                    for(int j=0; j<cNormals; ++j)
                    {                 
                        bCalcNormal = TRUE;
                        vertexNode1 = m_vecVertexList[j];                       
 
                        if(vertexNode->nId == vertexNode1->refVertex)
                        {                            
#if 1
                            if(vecCheckVect.size() <= 0)
                            {
                                normalVert += vertexNode1->normalVertex; 
                                //normalVert = normalVert.Normalize() + vertexNode1->normalVertex.Normalize();
                                vecCheckVect.push_back(vertexNode1->normalVertex);
                                //normalVert = normalVert.Normalize();
                                ++cntNVert;
                                continue;
                            }

                            for(int k=0; k<vecCheckVect.size(); ++k)
                            {
                                tempVec = vecCheckVect[k];
                                if(tempVec == vertexNode1->normalVertex)
                                {
                                    bCalcNormal = FALSE;
                                    break;
                                }
                            }             

                            if(bCalcNormal)
                            {
                                //normalVert = normalVert.Normalize() + vertexNode1->normalVertex.Normalize();
                                normalVert += vertexNode1->normalVertex; 
                                vecCheckVect.push_back(vertexNode1->normalVertex);
                                ++cntNVert;
                                //normalVert = normalVert.Normalize();
                            }
#else
                            normalVert += vertexNode1->normalVertex; 
                            //normalVert = normalVert.Normalize();
#endif
                        }
                    }
                    //normalVert = normalVert / cntNVert;

                    vertexNode->normalVertex = normalVert.Normalize();
                }
                else
                {
                    vertexNode->normalVertex = m_vecVertexList[vertexNode->refVertex]->normalVertex;
                }

            }           
        }
    }
#endif    
    

    // write nVerticeNormals
    WRITE_DWORD(pbCur, cNormals);

    // write verticesNormals
    for (int i = 0; i < cNormals; ++i)
    {
        vertexNode = m_vecVertexList[i];

        if(vertexNode != NULL)
        {
            vert = vertexNode->normalVertex;
            WRITE_POINT3(pbCur, vert);
            vertexNode = NULL;
        }        
    }

    // write nFace
    WRITE_DWORD(pbCur, m_nMaxFace);

    // write Face
    for(int i = 0; i < cFaces; ++i)
    {
        iFace = 0;

#if 0 
        while(iFace < cFaces)
        {
            faceNode = m_vecFaceList[iFace];
            if(faceNode->m_nId == i)
                break;

            ++iFace;
        }
#else
        faceNode = m_vecFaceList[i];
#endif

        if(faceNode != NULL)
        {
            WRITE_DWORD(pbCur, 3); //nFaceVertexIndices

            // face vertex indices
            if( bSwapTriOrder )
            {                
                WRITE_DWORD(pbCur, faceNode->m_VertexInx[0]);
                WRITE_DWORD(pbCur, faceNode->m_VertexInx[2]);
                WRITE_DWORD(pbCur, faceNode->m_VertexInx[1]);
            }
            else
            {
                WRITE_DWORD(pbCur, faceNode->m_VertexInx[0]);
                WRITE_DWORD(pbCur, faceNode->m_VertexInx[1]);
                WRITE_DWORD(pbCur, faceNode->m_VertexInx[2]);
            }
            faceNode = NULL;
        }        
    }

    // write Mesh
    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMMeshNormals,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );

    if (FAILED(hr))
    {        
        RELEASE(pDataObject);
        return false;
    }

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
    {        
        RELEASE(pDataObject);
        return false;
    }

    return true;
}
BOOL CXExpFrame::ExportTextureCoordinates(INode* pNode, Mesh *pMesh, SCropInfo *rgCropInfo, _PS ps, LPDIRECTXFILEDATA pParentDataObj)  
{
    LPDIRECTXFILEDATA pDataObject = NULL;
    PBYTE         pbData = NULL;
    PBYTE         pbCur = NULL;
    DWORD         cbSize;
    DWORD         cTexCoords;
    DWORD         iVertex;
    DWORD         iTextureIndex;
    HRESULT    hr = S_OK;
    float         fX;
    float         fY;
    DWORD         iMaterial;


    // if no tex coords, then don't add them
    //if( !pMeshData->m_bTexCoordsPresent )
    //    goto e_Exit;

    cTexCoords = m_nMaxVertex + 1;

    cbSize = sizeof(DWORD) //nTextureCoords
        + cTexCoords*2*sizeof(float); //texture coords

    pbCur = pbData = new BYTE[cbSize];
    if (pbData == NULL)
    {
        delete []pbData;
        return false;
    }

    WRITE_DWORD(pbCur, cTexCoords); //nTextureCoords

    CRefVertex *vertexNode;
    for (iVertex = 0; iVertex < cTexCoords; ++iVertex)
    {
        vertexNode = m_vecVertexList[iVertex];        

        iTextureIndex  = vertexNode->nTextureInx;
        iMaterial = vertexNode->m_MtlId;

        if( iTextureIndex == 0xFFFFFFFF ) // none present, or bad data
        {
            WRITE_FLOAT(pbCur, 0); //u
            WRITE_FLOAT(pbCur, 0); //v
        }
        else
        {
            fX = pMesh->tVerts[iTextureIndex].x;
            fY = (1.0f - pMesh->tVerts[iTextureIndex].y);

            //###danhoro $REMOVE: fixed texture tiling
            //fX = (fX > 1.0f) ? 1.0f : ((fX < 0.0f) ? 0.0f : fX);
            //fY = (fY > 1.0f) ? 1.0f : ((fY < 0.0f) ? 0.0f : fY);

            fX = (fX * rgCropInfo[iMaterial].fClipW) + rgCropInfo[iMaterial].fClipU;
            fY = (fY * rgCropInfo[iMaterial].fClipH) + rgCropInfo[iMaterial].fClipV;

            WRITE_FLOAT(pbCur, fX); //u
            WRITE_FLOAT(pbCur, fY); //v
        }
    }

    hr = ps.m_pxofsave->CreateDataObject(TID_D3DRMMeshTextureCoords,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );
    if (FAILED(hr))
    {
        RELEASE(pDataObject);

        delete []pbData;
        return false;
    }

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
    {
        RELEASE(pDataObject);

        delete []pbData;
        return false;
    }

   
    RELEASE(pDataObject);
    delete []pbData;
    
    return true;
}

BOOL CXExpFrame::ExportVertexDuplicationIndices(_PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT    hr = S_OK;
    LPDIRECTXFILEDATA pDataObject = NULL;
    PBYTE         pbData = NULL;
    PBYTE         pbCur = NULL;
    DWORD         cbSize;
    DWORD         cIndices;
    DWORD         iVertex;

    
    cIndices = m_nMaxVertex + 1;
    
    // if no new vertices, then don't add a record to the file
    if (m_nMaxVertexBeforeDuplication == m_nMaxVertex)
    {        
        return false;
    }

    cbSize = sizeof(DWORD) //nIndices
        + sizeof(DWORD) //nVerticesBeforeDuplication
        + cIndices*sizeof(DWORD); // array of indices

    pbCur = pbData = new BYTE[cbSize];
    if (pbData == NULL)
    {
        delete []pbData;
        return false;
    }

    WRITE_DWORD(pbCur, cIndices) // nIndices;
    WRITE_DWORD(pbCur, m_nMaxVertexBeforeDuplication+1) // nVerticesBeforeDuplication

    CRefVertex *vertexNode;
    for (iVertex = 0; iVertex < cIndices; ++iVertex)
    {        
        vertexNode = m_vecVertexList[iVertex];

        WRITE_DWORD(pbCur, vertexNode->refVertex); // index to original vertex without duplication.
    }

    hr = ps.m_pxofsave->CreateDataObject(DXFILEOBJ_VertexDuplicationIndices,
        NULL,
        NULL,
        cbSize,
        pbData,
        &pDataObject
        );
    if (FAILED(hr))
    {
        RELEASE(pDataObject);

        delete []pbData;
        return false;
    }

    hr = pParentDataObj->AddDataObject(pDataObject);
    if (FAILED(hr))
    {
        RELEASE(pDataObject);

        delete []pbData;
        return false;
    }
        
    RELEASE(pDataObject);
    delete []pbData;

    return true;
}

#if 1
BOOL CXExpFrame::ExportSkinData(INode *pNode, _PS ps, LPDIRECTXFILEDATA pParentDataObj)
{
    HRESULT hr = S_OK;
    IPhyVertexExport *pVertexExport;
    IPhyRigidVertex* pRigidVertexExport;
    IPhyBlendedRigidVertex *pBlendedRigidVertexExport;
    INode* pBoneNode;
    SSkinMap *psmSkinMap = NULL;
    Modifier* pPhyMod = NULL;
    IPhysiqueExport* pPhyExport = NULL;
    IPhyContextExport* pPhyContextExport = NULL;
    SBoneState *rgbsBoneData = NULL;
    SBoneState *rgbsBoneDataDups = NULL;
    SBoneState *pbsCurData = NULL;
    SBoneState *pbsOldData = NULL;
    DWORD iVertex;
    DWORD cVertices;
    DWORD iVertexType;
    DWORD cTotalBones;
    DWORD iBone;
    PBYTE pbCur;
    float fWeight;
    LPDIRECTXFILEDATA pDataObject = NULL;
    DWORD iIndex;
    Matrix3 matMesh;
    Matrix3 matOffset;
    Matrix3 matZero;
    PBYTE pbHeaderData = NULL;
    DWORD cbHeaderData;
    static bool shownError = false;

    matZero.Zero();
    pPhyMod = ps.m_pSkinData->FindPhysiqueModifier(pNode);
    if (pPhyMod != NULL)
    {
        // Get a Physique Export interface
        pPhyExport = (IPhysiqueExport *)pPhyMod->GetInterface(I_PHYINTERFACE);
        if (pPhyExport != NULL)
        { 
            pPhyExport->SetInitialPose(TRUE);
        }
        else
        {  // not all interfaces present, so ignore
            goto e_NoPhysique;
        }

        // get the init matrix for the mesh (used to mult by the bone matrices)
        //matMesh= pNode->GetNodeTM(0); 
        matMesh= pNode->GetObjTMAfterWSM(0);

        // For a given Object's INode get a
        // ModContext Interface from the Physique Export Interface:
        pPhyContextExport = (IPhyContextExport *)pPhyExport->GetContextInterface(pNode);
        if(pPhyContextExport == NULL)
        {
            // not all interfaces present, so ignore
            goto e_NoPhysique;
        }

        // should already have been converted to rigid with blending by Preprocess
        // convert to rigid with blending
        pPhyContextExport->ConvertToRigid(TRUE);
        pPhyContextExport->AllowBlending(TRUE);

        psmSkinMap = ps.m_pSkinData->m_ppc.GetSkinMap(pNode);
        if (psmSkinMap == NULL)
        {
            //XSkinExp_OutputDebugString("AddSkinData: Found physique info at save time, but not preprocess\n");
            hr = E_FAIL;
            goto e_Exit;
        }

        // now setup state used to fill arrays for output 
        rgbsBoneData = new SBoneState[psmSkinMap->m_cbiBones];
        if (rgbsBoneData == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // intialize each of the bone structures 
        for (iBone = 0; iBone < psmSkinMap->m_cbiBones; iBone++)
        {
            pbsCurData = &rgbsBoneData[iBone];

            pbsCurData->m_iCurIndex = 0;
            pbsCurData->m_cVertices = psmSkinMap->m_rgbiBones[iBone].m_cVertices;
            pbsCurData->m_pBoneNode = psmSkinMap->m_rgbiBones[iBone].m_pBoneNode;

            // allocate memory to pass to D3DXOF lib
            pbsCurData->m_cbData = sizeof(TCHAR*)
                + sizeof(DWORD) // numWeights
                + sizeof(DWORD) * pbsCurData->m_cVertices // array of vertex indices
                + sizeof(float) * pbsCurData->m_cVertices // array of weights
                + sizeof(float) * 16; // offset matrix

            pbCur = pbsCurData->m_pbData = new BYTE[pbsCurData->m_cbData];
            if (pbsCurData->m_pbData == NULL)
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }

            // fill first few entries, and remember pointers to the other arrays/matrix
            WRITE_PTCHAR(pbCur, pbsCurData->m_szBoneName);
            WRITE_DWORD(pbCur, pbsCurData->m_cVertices);

            pbsCurData->m_rgdwIndices = (DWORD*)pbCur;
            pbCur += sizeof(DWORD) * pbsCurData->m_cVertices;

            pbsCurData->m_rgfWeights = (float*)pbCur;
            pbCur += sizeof(float) * pbsCurData->m_cVertices;

            pbsCurData->m_pmatOffset = (D3DXMATRIX*)pbCur;

            // EDITED : 바이패드 없이 메쉬만 뽑을대..
            //
            // compute the mat offset
            if( pbsCurData->m_pBoneNode != NULL )
            {
                matOffset= pbsCurData->m_pBoneNode->GetNodeTM(0);

                //matOffset.Invert();
            }
            else
            {
                matOffset.IdentityMatrix();
            }
            //

            //###danhoro $REMOVE this doesnt seem to really work as well as setting the physique to initial pose
            //int rval = pPhyExport->GetInitNodeTM(pbsCurData->m_pBoneNode, matOffset);
            //if (rval)
            //{
            //    MessageBox(NULL, "This mesh was skinned using an older version of Character Studio. Mesh may not be exported correctly",
            //               "X File export Warning", MB_OK);
            //}

            //matOffset.Invert();
            matOffset = matMesh * Inverse(matOffset);
            
            WRITE_MATRIX4_FROM_MATRIX3(pbCur, matOffset);
        }

        cVertices = pPhyContextExport->GetNumberVertices();
        for (iVertex = 0; iVertex < cVertices; iVertex++ )
        {
            pVertexExport = (IPhyVertexExport *)pPhyContextExport->GetVertexInterface(iVertex);    
            if (pVertexExport == NULL)
            {
                //XSkinExp_OutputDebugString("AddSkinData: Couldn't get physique vertex export interface!");
                hr = E_FAIL;
                goto e_Exit;
            }

            // What kind of vertices are these?
            iVertexType = pVertexExport->GetVertexType();


            if( iVertexType == RIGID_TYPE )
            {
                pRigidVertexExport = (IPhyRigidVertex *)pVertexExport;//pPhyContextExport->GetVertexInterface(iVertex);

                // Get the vertex info!

                pBoneNode = pRigidVertexExport->GetNode();

                pbsCurData = ps.m_pSkinData->FindBoneData(pBoneNode, rgbsBoneData, psmSkinMap->m_cbiBones);
                if (pbsCurData == NULL)
                {
                    assert(0);
                    //XSkinExp_OutputDebugString("AddSkinData: Bone node not found on second pass\n");
                    hr = E_FAIL;
                    goto e_Exit;
                }

                pbsCurData->m_rgdwIndices[pbsCurData->m_iCurIndex] = iVertex;
                pbsCurData->m_rgfWeights[pbsCurData->m_iCurIndex] = 1.0f;
                pbsCurData->m_iCurIndex += 1;

            }
            else
            {
                assert( iVertexType == RIGID_BLENDED_TYPE );

                pBlendedRigidVertexExport = (IPhyBlendedRigidVertex *)pVertexExport;//pPhyContextExport->GetVertexInterface(iVertex);


                // How many nodes affect his vertex?
                cTotalBones = pBlendedRigidVertexExport->GetNumberNodes();
                for (iBone = 0; iBone < cTotalBones; iBone++ )
                {
                    pBoneNode = pBlendedRigidVertexExport->GetNode(iBone);

                    pbsCurData = ps.m_pSkinData->FindBoneData(pBoneNode, rgbsBoneData, psmSkinMap->m_cbiBones);
                    if (pbsCurData == NULL)
                    {
                        assert(0);
                        //XSkinExp_OutputDebugString("AddSkinData: Bone node not found on second pass\n");
                        hr = E_FAIL;
                        goto e_Exit;
                    }

                    fWeight = pBlendedRigidVertexExport->GetWeight(iBone);

                    // first see if it is a repeat weight, is so just add to previous
                    if ((pbsCurData->m_iCurIndex > 0) && (pbsCurData->m_rgdwIndices[pbsCurData->m_iCurIndex-1] == iVertex))
                    {
                        pbsCurData->m_rgfWeights[pbsCurData->m_iCurIndex-1] += fWeight;
                    }
                    else
                    {
                        pbsCurData->m_rgdwIndices[pbsCurData->m_iCurIndex] = iVertex;
                        pbsCurData->m_rgfWeights[pbsCurData->m_iCurIndex] = fWeight;
                        pbsCurData->m_iCurIndex += 1;
                    }                    
                }
            }

            pPhyContextExport->ReleaseVertexInterface( pVertexExport );    
        }
    }
e_NoPhysique:



    if (rgbsBoneData != NULL)
    {
        assert(psmSkinMap != NULL);

        // now deal with the wonderful world of duplicated indices
        rgbsBoneDataDups = new SBoneState[psmSkinMap->m_cbiBones];
        if (rgbsBoneDataDups == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // first calculate new lengths for the bone weight arrays
        for (iBone = 0; iBone < psmSkinMap->m_cbiBones; iBone++)
        {
            pbsOldData = &rgbsBoneData[iBone];
            pbsCurData = &rgbsBoneDataDups[iBone];

            pbsCurData->m_cVertices = pbsOldData->m_cVertices;
            for (iIndex = 0; iIndex < pbsOldData->m_cVertices; ++iIndex)
            {                
                iVertex = pbsOldData->m_rgdwIndices[iIndex];
                DWORD iVertexInx = m_vecVertexList[iVertex]->refVertex;
                
                                
                for(int i=0; i<m_vecVertexList.size(); ++i)
                {                    
                    DWORD refVertexId = m_vecVertexList[i]->refVertex;

                    if(i != iVertex && iVertexInx == refVertexId)
                    {                        
                        pbsCurData->m_cVertices += 1;                        
                    }
                }
            }
        }

        // next build 
        for (iBone = 0; iBone < psmSkinMap->m_cbiBones; iBone++)
        {
            pbsOldData = &rgbsBoneData[iBone];
            pbsCurData = &rgbsBoneDataDups[iBone];

            pbsCurData->m_pBoneNode = pbsOldData->m_pBoneNode;
            pbsCurData->m_iCurIndex = 0;

            // allocate memory to pass to D3DXOF lib
            pbsCurData->m_cbData = sizeof(TCHAR*)
                + sizeof(DWORD) // numWeights
                + sizeof(DWORD) * pbsCurData->m_cVertices // array of vertex indices
                + sizeof(float) * pbsCurData->m_cVertices // array of weights
                + sizeof(float) * 16; // offset matrix

            pbCur = pbsCurData->m_pbData = new BYTE[pbsCurData->m_cbData];

            // EDITED :
            //
            if( pbsCurData->m_pBoneNode != NULL )
            {
                pbsCurData->m_szBoneName = g_stStrings.CreateNiceString(pbsCurData->m_pBoneNode->GetName());
            }
            //

            if ((pbsCurData->m_pbData == NULL) || (pbsCurData->m_szBoneName == NULL))
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }

            // fill first few entries, and remember pointers to the other arrays/matrix
            WRITE_PTCHAR(pbCur, pbsCurData->m_szBoneName);
            WRITE_DWORD(pbCur, pbsCurData->m_cVertices);

            pbsCurData->m_rgdwIndices = (DWORD*)pbCur;
            pbCur += sizeof(DWORD) * pbsCurData->m_cVertices;

            pbsCurData->m_rgfWeights = (float*)pbCur;
            pbCur += sizeof(float) * pbsCurData->m_cVertices;

            pbsCurData->m_pmatOffset = (D3DXMATRIX*)pbCur;

            // already loaded above, copy from the original state data
            *pbsCurData->m_pmatOffset = *pbsOldData->m_pmatOffset;


            // now that we have the new states set up, copy the data from the old states
            for (iIndex = 0; iIndex < pbsOldData->m_cVertices; ++iIndex)
            {
                iVertex = pbsOldData->m_rgdwIndices[iIndex];
                DWORD iVertexInx = m_vecVertexList[iVertex]->refVertex;

                pbsCurData->m_rgdwIndices[pbsCurData->m_iCurIndex] = m_vecVertexList[iVertex]->nId;
                pbsCurData->m_rgfWeights[pbsCurData->m_iCurIndex] = pbsOldData->m_rgfWeights[iIndex];
                pbsCurData->m_iCurIndex += 1;                

                for(int i=0; i<m_vecVertexList.size(); ++i)
                {
                    DWORD refVertexId = m_vecVertexList[i]->refVertex;

                    if(i != iVertex && iVertexInx == refVertexId)
                    {
                        pbsCurData->m_rgdwIndices[pbsCurData->m_iCurIndex] = m_vecVertexList[i]->nId;
                        pbsCurData->m_rgfWeights[pbsCurData->m_iCurIndex] = pbsOldData->m_rgfWeights[iIndex];
                        pbsCurData->m_iCurIndex += 1;
                    }
                }   
            }
        }

        // now that we do have skin data to put in the file, add a skinning header record
        cbHeaderData = sizeof(WORD) * 3;
        pbCur = pbHeaderData = new BYTE[cbHeaderData];
        if (pbHeaderData == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        WRITE_WORD(pbCur, ps.m_pSkinData->m_ppc.m_cMaxWeightsPerVertex);
        WRITE_WORD(pbCur, ps.m_pSkinData->m_ppc.m_cMaxWeightsPerFace);
        WRITE_WORD(pbCur, psmSkinMap->m_cbiBones);

        hr = ps.m_pxofsave->CreateDataObject(DXFILEOBJ_XSkinMeshHeader,
            NULL,
            NULL,
            cbHeaderData,
            pbHeaderData,
            &pDataObject
            );
        hr = pParentDataObj->AddDataObject(pDataObject);
        if (FAILED(hr))
            goto e_Exit;

        RELEASE(pDataObject);

        // now actually output the prepared buffers
        for (iBone = 0; iBone < psmSkinMap->m_cbiBones; ++iBone)
        {
            assert(rgbsBoneData[iBone].m_iCurIndex == rgbsBoneData[iBone].m_cVertices);
            assert(rgbsBoneDataDups[iBone].m_iCurIndex == rgbsBoneDataDups[iBone].m_cVertices);

            hr = ps.m_pxofsave->CreateDataObject(DXFILEOBJ_SkinWeights,
                NULL,
                NULL,
                rgbsBoneDataDups[iBone].m_cbData,
                rgbsBoneDataDups[iBone].m_pbData,
                &pDataObject
                );
            if (FAILED(hr))
                goto e_Exit;

            hr = pParentDataObj->AddDataObject(pDataObject);
            if (FAILED(hr))
                goto e_Exit;

            RELEASE(pDataObject);
        }
    }

e_Exit:
    if (pPhyExport != NULL)
    {
        pPhyExport->SetInitialPose(FALSE);
    }


    delete []pbHeaderData;
    delete []rgbsBoneData;
    delete []rgbsBoneDataDups;
    return hr;
}
#endif