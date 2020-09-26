#include "XFileExp.h"
#include "XExpUtil.h"
#include "XExpFunc.h"

struct SAnimFrameAge
{
    DWORD	m_dwStartFrame;
    DWORD	m_dwEndFrame;
};

struct SAnimInfo
{
    DWORD dwTime;
    DWORD dwNumValues;
    float rgfValues[16];
};

struct SBoneInfo
{
    INode                       *m_pBoneNode;
    DWORD                       m_cVertices;
};

// structure used to map an mesh to a bone info structure
struct SSkinMap
{
    SSkinMap()
        :m_pMeshNode(NULL), m_rgbiBones(NULL), m_cbiBones(0), m_cbiBonesMax(0) {}
        ~SSkinMap()
        { delete []m_rgbiBones; }

        INode                       *m_pMeshNode;

        SBoneInfo                   *m_rgbiBones;
        DWORD                       m_cbiBones;
        DWORD                       m_cbiBonesMax;

        SBoneInfo *FindBone(INode *pBoneNode)
        {
            SBoneInfo *pbi = NULL;
            DWORD iBone;

            for (iBone = 0; iBone < m_cbiBones; iBone++)
            {
                if (pBoneNode == m_rgbiBones[iBone].m_pBoneNode)
                {
                    pbi = &m_rgbiBones[iBone];
                    break;
                }
            }

            return pbi;
        }

        HRESULT AddBone(INode *pBoneNode, SBoneInfo **ppbiBoneInfo)
        {
            HRESULT hr = S_OK;
            SBoneInfo *rgbiTemp;

            // reallocate if neccessary
            if (m_cbiBones == m_cbiBonesMax)
            {
                m_cbiBonesMax = max(1, m_cbiBonesMax);
                m_cbiBonesMax *= 2;

                rgbiTemp = m_rgbiBones;
                m_rgbiBones = new SBoneInfo[m_cbiBonesMax];
                if (m_rgbiBones == NULL)
                {
                    m_rgbiBones = rgbiTemp;
                    hr = E_OUTOFMEMORY;
                    goto e_Exit;
                }

                if (m_cbiBones > 0)
                {
                    memcpy(m_rgbiBones, rgbiTemp, m_cbiBones * sizeof(SBoneInfo));
                }

                delete []rgbiTemp;
            }

            // not initialize the next bone in the array and return a pointer to it

            m_rgbiBones[m_cbiBones].m_cVertices = 0;
            m_rgbiBones[m_cbiBones].m_pBoneNode = pBoneNode;

            *ppbiBoneInfo = &m_rgbiBones[m_cbiBones];

            m_cbiBones += 1;

e_Exit:
            return hr;
        }
};

struct SPreprocessContext
{
    SPreprocessContext()
        :
    m_pInterface(NULL),
        m_rgpsmSkinMaps(NULL), 
        m_cpsmSkinMaps(NULL), 
        m_cpsmSkinMapsMax(0), 
        m_cMaxWeightsPerVertex(0), 
        m_cMaxWeightsPerFace(0),
        m_cNodes(0) {}

        ~SPreprocessContext()
        { delete []m_rgpsmSkinMaps; }

        Interface*                  m_pInterface;
        BOOL                        m_bSaveSelection;

        SSkinMap                    **m_rgpsmSkinMaps;
        DWORD                       m_cpsmSkinMaps;
        DWORD                       m_cpsmSkinMapsMax;

        DWORD                       m_cMaxWeightsPerVertex;
        DWORD                       m_cMaxWeightsPerFace;

        DWORD                       m_cNodes;

        SSkinMap *GetSkinMap(INode *pMeshNode)
        {
            SSkinMap *psm = NULL;
            DWORD iMesh;

            for (iMesh = 0; iMesh < m_cpsmSkinMaps; iMesh++)
            {
                if (pMeshNode == m_rgpsmSkinMaps[iMesh]->m_pMeshNode)
                {
                    psm = m_rgpsmSkinMaps[iMesh];
                    break;
                }
            }

            return psm;
        }
};

#if 0 
struct SSaveContext
{
    SSaveContext()
        :m_rgpsmSkinMaps(NULL), m_pAnimationSet(NULL) {}

        ~SSaveContext()
        { delete []m_rgpsmSkinMaps; }

        LPDIRECTXFILESAVEOBJECT     m_pxofsave;
        DXFILEFORMAT                m_xFormat;
        DWORD                       m_iTime;
        BOOL                        m_bSaveSelection;
        BOOL                        m_bSavePatchData;
        BOOL                        m_bSaveAnimationData;
        BOOL                        m_bLoopingAnimationData;
        DWORD                       m_iAnimSamplingRate;
        DWORD                       m_cMaxWeightsPerVertex;
        DWORD                       m_cMaxWeightsPerFace;

        SSkinMap                    **m_rgpsmSkinMaps;
        DWORD                       m_cpsmSkinMaps;

        LPDIRECTXFILEDATA           m_pAnimationSet;
        Interface                   *m_pInterface;

        DWORD                       m_cNodes;
        DWORD                       m_cNodesCur;
        INode                       **m_rgpnNodes;

        CStringTable				m_stStrings;

        SSkinMap *GetSkinMap(INode *pMeshNode)
        {
            SSkinMap *psm = NULL;
            DWORD iMesh;

            for (iMesh = 0; iMesh < m_cpsmSkinMaps; iMesh++)
            {
                if (pMeshNode == m_rgpsmSkinMaps[iMesh]->m_pMeshNode)
                {
                    psm = m_rgpsmSkinMaps[iMesh];
                    break;
                }
            }

            return psm;
        }
};
#endif

// structe used by AddSkinData to convert skin data from per vertex to per bone
struct SBoneState
{
    SBoneState()
        :m_pbData(NULL), m_szBoneName(NULL) {}
        ~SBoneState()
        { 
            delete []m_pbData; 

            // Bone name is allocated out of a string pool because it has to be deleted after
            //   the data has been saved to disk
            //delete []m_szBoneName; 
        }

        // info computed up front (note m_rgdwIndices, m_rgfWeights and m_pmatOFfset point into pbData)
        INode *m_pBoneNode;
        DWORD m_cVertices;
        DWORD m_cbData;
        PBYTE m_pbData;
        char *m_szBoneName;
        DWORD *m_rgdwIndices;
        float *m_rgfWeights;
        D3DXMATRIX *m_pmatOffset;

        // current index of last added vertex index
        DWORD m_iCurIndex;
};


class CXExpSkinData
{
public:
    CXExpSkinData(Interface *ip);
    ~CXExpSkinData() {};

private:
    LPDIRECTXFILESAVEOBJECT     m_pxofsave;
    std::vector<INode*>     m_rgpnNodes;
    LPDIRECTXFILEDATA       m_pAnimationSet;
    DWORD                   m_iAnimSamplingRate;

public:
    SPreprocessContext  m_ppc;
    //SSaveContext        m_ssc;

public:
    void        PushNode(INode *pNode) { m_rgpnNodes.push_back((pNode)); }
    void        SetAniInfo(LPDIRECTXFILESAVEOBJECT fsave);
    LPDIRECTXFILEDATA   GetAniSet() { return m_pAnimationSet; }
    BOOL        Preprocess(INode *pNode);
    Modifier*   FindPhysiqueModifier (INode* nodePtr);
    SBoneState *FindBoneData(INode *pBoneNode, SBoneState *rgbsBoneData, DWORD cBones);
    HRESULT     ExportAnimationSet(TCHAR *strAnimName);

private:
    HRESULT _Preprocess(SPreprocessContext *ppc, INode *pNode, DWORD &numPhysique);
};