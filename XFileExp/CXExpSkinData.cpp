#include "CXExpSkinData.h"

#include "rmxfguid.h"

extern CStringTable         g_stStrings;
extern SDialogOptions g_DlgOptions;
SAnimFrameAge g_AnimFrameAge;

CXExpSkinData::CXExpSkinData(Interface *ip)
{
    m_ppc.m_pInterface = ip;
    m_pAnimationSet = NULL;
    m_pxofsave = NULL;
    m_iAnimSamplingRate = 30;

}

SBoneState *CXExpSkinData::FindBoneData(INode *pBoneNode, SBoneState *rgbsBoneData, DWORD cBones)
{
    DWORD iBone;
    SBoneState *pbs = NULL;

    for (iBone = 0; iBone < cBones; iBone++)
    {
        if (rgbsBoneData[iBone].m_pBoneNode == pBoneNode)
        {
            pbs = &rgbsBoneData[iBone];
            break;
        }
    }

    return pbs;
}

// ================================================== FindPhysiqueModifier()
// Find if a given node contains a Physique Modifier
// DerivedObjectPtr requires you include "modstack.h" from the MAX SDK
Modifier* CXExpSkinData::FindPhysiqueModifier (INode* nodePtr)
{
    // Get object from node. Abort if no object.
    Object* ObjectPtr = nodePtr->GetObjectRef();


    if ( NULL == ObjectPtr)
    {
        return NULL;
    }

    // Is derived object ?
    if (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        // Yes -> Cast.
        IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);

        // Iterate over all entries of the modifier stack.
        int ModStackIndex = 0;
        while (ModStackIndex < DerivedObjectPtr->NumModifiers())
        {
            // Get current modifier.
            Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
            Class_ID clsid = ModifierPtr->ClassID();
            // Is this Physique ?
            if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
            {
                // Yes -> Exit.
                return ModifierPtr;
            }

            // Next modifier stack entry.
            ModStackIndex++;
        }
    }

    // Not found.
    return NULL;
}

void CXExpSkinData::SetAniInfo(LPDIRECTXFILESAVEOBJECT fsave)
{
    m_pxofsave = fsave;

    
}

BOOL CXExpSkinData::Preprocess(INode *pNode)
{
    DWORD numPhysique = 0;

    _Preprocess(&m_ppc, pNode, numPhysique);

    if(numPhysique > 0)
        return TRUE;
    else
        return FALSE;
}

HRESULT CXExpSkinData::_Preprocess(SPreprocessContext *ppc, INode *pNode, DWORD &numPhysique)
{
    HRESULT hr = S_OK;
    DWORD cChildren;
    DWORD iChild;
    Object* pObj;
    SSkinMap **rgpsmTemp = NULL;
    IPhyVertexExport *pVertexExport = NULL;
    IPhyRigidVertex* pRigidVertexExport = NULL;
    IPhyBlendedRigidVertex *pBlendedRigidVertexExport = NULL;
    INode* pBoneNode;
    SSkinMap *psmSkinMap;
    Modifier* pPhyMod = NULL;
    IPhysiqueExport* pPhyExport = NULL;
    IPhyContextExport* pPhyContextExport = NULL;
    DWORD iVertex;
    DWORD cVertices;
    DWORD iVertexType;
    SBoneInfo *pbi;
    DWORD cTotalBones;
    DWORD iBone;
    DWORD cpnBonesSeen;
    DWORD cpnBonesSeenMax;
    INode **rgpnBonesSeen = NULL;
    INode **rgpnTemp;
    BOOL bBoneSeen;
    DWORD iBoneSeen;
    BOOL bDeleteTriObject = false;
    TriObject *pTriObject = NULL;
    Mesh *pMesh;
    DWORD iFace;
    DWORD iPoint;

    ppc->m_cNodes += 1;

    if (IsExportableMesh(pNode, &pObj)  && !(g_DlgOptions.m_bSelection && !pNode->Selected()))
    {
        // first see if physique is present
        pPhyMod = FindPhysiqueModifier(pNode);
        if (pPhyMod != NULL)
        {            

            // Get a Physique Export interface
            pPhyExport = (IPhysiqueExport *)pPhyMod->GetInterface(I_PHYINTERFACE);
            if (pPhyExport == NULL)
            {   // not all interfaces present, so ignore
                goto e_NoPhysique;
            }
            // For a given Object's INode get a
            // ModContext Interface from the Physique Export Interface:
            pPhyContextExport = (IPhyContextExport *)pPhyExport->GetContextInterface(pNode);
            if(pPhyContextExport == NULL)
            {
                // not all interfaces present, so ignore
                goto e_NoPhysique;
            }

            ++numPhysique;

            // convert to rigid with blending
            pPhyContextExport->AllowBlending(TRUE);
            pPhyContextExport->ConvertToRigid(TRUE);

            psmSkinMap = new SSkinMap;
            if (psmSkinMap == NULL)
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }

            // realloc if necessary
            if (ppc->m_cpsmSkinMaps == ppc->m_cpsmSkinMapsMax)
            {
                rgpsmTemp = ppc->m_rgpsmSkinMaps;

                ppc->m_cpsmSkinMapsMax = max(1, ppc->m_cpsmSkinMapsMax);
                ppc->m_cpsmSkinMapsMax = ppc->m_cpsmSkinMapsMax * 2;
                ppc->m_rgpsmSkinMaps = new SSkinMap*[ppc->m_cpsmSkinMapsMax];
                if (ppc->m_rgpsmSkinMaps == NULL)
                {
                    ppc->m_rgpsmSkinMaps = rgpsmTemp;
                    hr = E_OUTOFMEMORY;
                    goto e_Exit;
                }

                if (ppc->m_cpsmSkinMaps > 0)
                {
                    memcpy(ppc->m_rgpsmSkinMaps, rgpsmTemp, sizeof(SSkinMap*) * ppc->m_cpsmSkinMaps);
                }

                delete []rgpsmTemp;
            }
            ppc->m_rgpsmSkinMaps[ppc->m_cpsmSkinMaps] = psmSkinMap;
            ppc->m_cpsmSkinMaps += 1;

            // init the map
            psmSkinMap->m_pMeshNode = pNode;
            psmSkinMap->m_cbiBonesMax = 30;
            psmSkinMap->m_rgbiBones = new SBoneInfo[psmSkinMap->m_cbiBonesMax];
            if (psmSkinMap->m_rgbiBones == NULL)
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }

            // init duplication removal for redundant weights
            cpnBonesSeenMax = 30;
            cpnBonesSeen = 0;
            rgpnBonesSeen = new INode*[cpnBonesSeenMax];
            if (rgpnBonesSeen == NULL)
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }

            cVertices = pPhyContextExport->GetNumberVertices();
            for (iVertex = 0; iVertex < cVertices; iVertex++ )
            {
                cpnBonesSeen = 0;

                // What kind of vertices are these?
                pVertexExport = (IPhyVertexExport *)pPhyContextExport->GetVertexInterface(iVertex);    
                if (pVertexExport == NULL)
                {
                    //XSkinExp_OutputDebugString("Preprocess: Couldn't get physique vertex export interface!");
                    hr = E_FAIL;
                    goto e_Exit;
                }
                else
                {    
                    // What kind of vertices are these?
                    iVertexType = pVertexExport->GetVertexType();

                    //pPhyContextExport->ReleaseVertexInterface( pVertexExport );    

                    if( iVertexType == RIGID_TYPE )
                    {
                        pRigidVertexExport = (IPhyRigidVertex *)pVertexExport;//pPhyContextExport->GetVertexInterface(iVertex);


                        pBoneNode = pRigidVertexExport->GetNode();

                        pbi = psmSkinMap->FindBone(pBoneNode);
                        if (pbi == NULL)
                        {
                            hr = psmSkinMap->AddBone(pBoneNode, &pbi);
                            if (FAILED(hr))
                                goto e_Exit;
                        }

                        pbi->m_cVertices += 1;

                        ppc->m_cMaxWeightsPerVertex = max(1, ppc->m_cMaxWeightsPerVertex);

                    }
                    else
                    {
                        assert( iVertexType == RIGID_BLENDED_TYPE );

                        pBlendedRigidVertexExport = (IPhyBlendedRigidVertex *)pVertexExport;//pPhyContextExport->GetVertexInterface(iVertex);

                        // How many nodes affect his vertex?
                        cTotalBones = pBlendedRigidVertexExport->GetNumberNodes();
                        cpnBonesSeen = 0;
                        for (iBone = 0; iBone < cTotalBones; iBone++ )
                        {
                            pBoneNode = pBlendedRigidVertexExport->GetNode(iBone);

                            // first see if the bone has already been seen
                            bBoneSeen = FALSE;
                            for (iBoneSeen = 0; iBoneSeen < cpnBonesSeen; iBoneSeen++)
                            {
                                if (rgpnBonesSeen[iBoneSeen] == pBoneNode)
                                {
                                    bBoneSeen = TRUE;
                                    break;
                                }
                            }

                            // if not seen, collect stats and add to already seen
                            if (!bBoneSeen)
                            {
                                pbi = psmSkinMap->FindBone(pBoneNode);
                                if (pbi == NULL)
                                {
                                    hr = psmSkinMap->AddBone(pBoneNode, &pbi);
                                    if (FAILED(hr))
                                        goto e_Exit;
                                }
                                pbi->m_cVertices += 1;

                                if (cpnBonesSeen >= cpnBonesSeenMax)
                                {
                                    rgpnTemp = rgpnBonesSeen;
                                    cpnBonesSeenMax *= 2;

                                    rgpnBonesSeen = new INode*[cpnBonesSeenMax];
                                    if (rgpnBonesSeen == NULL)
                                    {
                                        rgpnBonesSeen = rgpnTemp;
                                        hr = E_OUTOFMEMORY;
                                        goto e_Exit;
                                    }

                                    memcpy(rgpnBonesSeen, rgpnTemp, cpnBonesSeen * sizeof(INode*));
                                    delete []rgpnTemp;
                                }

                                rgpnBonesSeen[cpnBonesSeen] = pBoneNode;
                                cpnBonesSeen += 1;
                            }
                        }

                        // actualNum... accounts for duplicated weights to same transform node
                        // that are combined automatically above
                        ppc->m_cMaxWeightsPerVertex = max(cpnBonesSeen, ppc->m_cMaxWeightsPerVertex);

                    }

                    pPhyContextExport->ReleaseVertexInterface( pVertexExport);


                }
            }


            // now figure out the max number of weights per face

            pTriObject = GetTriObjectFromObjRef(pObj, &bDeleteTriObject);
            if (pTriObject == NULL) 
            {
                //XSkinExp_OutputDebugString("Preprocess: Physique info, but no mesh");
                hr = E_FAIL;
                goto e_Exit;
            }

            pMesh = &(pTriObject->mesh);

            for (iFace = 0; iFace < pMesh->numFaces; iFace++)
            {
                cpnBonesSeen = 0;
                for (iPoint = 0; iPoint < 3; iPoint++ )
                {
                    iVertex = pMesh->faces[iFace].v[iPoint];

                    pVertexExport = (IPhyVertexExport *)pPhyContextExport->GetVertexInterface(iVertex);    
                    if (pVertexExport == NULL)
                    {
                        //XSkinExp_OutputDebugString("Preprocess: Couldn't get physique vertex export interface!");
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

                        pbi = psmSkinMap->FindBone(pBoneNode);
                        if (pbi == NULL)
                        {
                            hr = psmSkinMap->AddBone(pBoneNode, &pbi);
                            if (FAILED(hr))
                                goto e_Exit;
                        }

                        // first see if the bone has already been seen
                        bBoneSeen = FALSE;
                        for (iBoneSeen = 0; iBoneSeen < cpnBonesSeen; iBoneSeen++)
                        {
                            if (rgpnBonesSeen[iBoneSeen] == pBoneNode)
                            {
                                bBoneSeen = TRUE;
                                break;
                            }
                        }

                        // if not seen, collect stats and add to already seen
                        if (!bBoneSeen)
                        {
                            if (cpnBonesSeen >= cpnBonesSeenMax)
                            {
                                rgpnTemp = rgpnBonesSeen;
                                cpnBonesSeenMax *= 2;

                                rgpnBonesSeen = new INode*[cpnBonesSeenMax];
                                if (rgpnBonesSeen == NULL)
                                {
                                    rgpnBonesSeen = rgpnTemp;
                                    hr = E_OUTOFMEMORY;
                                    goto e_Exit;
                                }

                                memcpy(rgpnBonesSeen, rgpnTemp, cpnBonesSeen * sizeof(INode*));
                                delete []rgpnTemp;
                            }

                            rgpnBonesSeen[cpnBonesSeen] = pBoneNode;
                            cpnBonesSeen += 1;
                        }

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

                            // first see if the bone has already been seen
                            bBoneSeen = FALSE;
                            for (iBoneSeen = 0; iBoneSeen < cpnBonesSeen; iBoneSeen++)
                            {
                                if (rgpnBonesSeen[iBoneSeen] == pBoneNode)
                                {
                                    bBoneSeen = TRUE;
                                    break;
                                }
                            }

                            // if not seen, collect stats and add to already seen
                            if (!bBoneSeen)
                            {
                                if (cpnBonesSeen >= cpnBonesSeenMax)
                                {
                                    rgpnTemp = rgpnBonesSeen;
                                    cpnBonesSeenMax *= 2;

                                    rgpnBonesSeen = new INode*[cpnBonesSeenMax];
                                    if (rgpnBonesSeen == NULL)
                                    {
                                        rgpnBonesSeen = rgpnTemp;
                                        hr = E_OUTOFMEMORY;
                                        goto e_Exit;
                                    }

                                    memcpy(rgpnBonesSeen, rgpnTemp, cpnBonesSeen * sizeof(INode*));
                                    delete []rgpnTemp;
                                }

                                rgpnBonesSeen[cpnBonesSeen] = pBoneNode;
                                cpnBonesSeen += 1;
                            }
                        }
                    }
                    pPhyContextExport->ReleaseVertexInterface( pVertexExport );  
                }

                ppc->m_cMaxWeightsPerFace = max(cpnBonesSeen, ppc->m_cMaxWeightsPerFace);
            }
        }

e_NoPhysique:;
    }

    cChildren = pNode->NumberOfChildren();
    for (iChild = 0; iChild < cChildren; iChild++)
    {
        // enumerate the child
        hr = _Preprocess(ppc, pNode->GetChildNode(iChild), numPhysique);
        if (FAILED(hr))
            goto e_Exit;        
    }

e_Exit:
    if (bDeleteTriObject)
    {
        delete pTriObject;
    }

    delete []rgpnBonesSeen;
    return hr;
}


HRESULT CXExpSkinData::ExportAnimationSet(TCHAR *strAnimName)
{ 
    HRESULT hr = S_OK;
#if 1
    DWORD cKeys;
    PBYTE pbData = NULL;
    PBYTE pbCur;
    DWORD cTicksPerFrame;
    DWORD iCurTime;
    DWORD iCurKey;
    Matrix3 matFirstTM;
    Matrix3 matTM;
    Matrix3 matRelativeTM;
    Matrix3 matParentTM;
    Interval interval;
    DWORD iNode;
    INode *pNode;
    DWORD cbSize;
    DWORD iKey;
    DWORD cCurrentKeys;
    SAnimInfo *rgaiAnimData = NULL;
    SAnimInfo *rgaiAnimDataCur;
    LPDIRECTXFILEDATA pAnimation = NULL;
    LPDIRECTXFILEDATA pAnimationKeys = NULL;
    char *szName;
    BOOL bAddEndKey = FALSE;
    DWORD iInterval;
    DWORD iStartTime;
    DWORD iEndTime;
    DWORD cFrameRate;

    // find the animation info from the interface
    interval = m_ppc.m_pInterface->GetAnimRange();
    cTicksPerFrame = GetTicksPerFrame();
    cFrameRate = GetFrameRate();
    iStartTime = interval.Start();
    iEndTime = interval.End();

    iInterval = (cTicksPerFrame * cFrameRate) / m_iAnimSamplingRate;

    iStartTime	= g_AnimFrameAge.m_dwStartFrame * iInterval;
    iEndTime	= g_AnimFrameAge.m_dwEndFrame * iInterval;

    cKeys = (iEndTime - iStartTime) / iInterval;

    // add a key for the last frame at iEndTime, or the one added if iEndTime is not directly touched
    //cKeys += 1;

    // add a key to give the final frame a length (difference between Max and X file keyframe specification)
    cKeys += 1;

    // if the sampling frequency doesn't end directly on 
    //   on the end time, then add a partial end key
    if (((iEndTime - iStartTime) % iInterval) != 0)
    {
        bAddEndKey = TRUE;
    }

    rgaiAnimData = new SAnimInfo[m_ppc.m_cNodes*cKeys];
    if (rgaiAnimData == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    for (iCurKey = 0, iCurTime = iStartTime; iCurTime <= iEndTime; iCurTime += iInterval, iCurKey++ )
    {
        // 진행률 표시
        int iProcess;            
        iProcess = ((float)(iStartTime - iCurTime) / (float)(iEndTime - iStartTime)) * 30.0f;
        iProcess += 60;
        m_ppc.m_pInterface->ProgressUpdate(iProcess);

        for (iNode = 0; iNode < m_ppc.m_cNodes; iNode++)
        {
            if(iNode >= m_rgpnNodes.size())
                break;

            pNode = m_rgpnNodes[iNode];

            if((g_DlgOptions.m_bSelection && !pNode->Selected()))
            {
                continue;
            }

            //iCurTime = iFrame * (cTicksPerFrame / 2);
            pNode->EvalWorldState(iCurTime );


            //matTM = pNode->GetNodeTM(iCurTime);
            if( g_DlgOptions.m_bChildrenNotAttached == TRUE )
                matTM = pNode->GetObjectTM(iCurTime);	//offset이 적용된 tm
            else
                matTM = pNode->GetNodeTM(iCurTime);	//offset이 적용되지 않은 tm



            if (pNode->GetParentNode() == NULL)
            {
                matRelativeTM = matTM;

            }
            else
            {
                
                pNode->GetParentNode()->EvalWorldState(iCurTime );

                if( g_DlgOptions.m_bChildrenNotAttached == TRUE )
                    matParentTM = pNode->GetParentNode()->GetObjectTM(iCurTime);	//offset이 적용된 tm
                else
                    matParentTM = pNode->GetParentNode()->GetNodeTM(iCurTime);	//offset이 적용되지 않은 tm
                //matParentTM = pNode->GetParentNode()->GetNodeTM(iCurTime );

                if( matTM == matParentTM )
                {
                    matRelativeTM.IdentityMatrix();
                }
                else
                {
                    Matrix3 inverseMat = Inverse(matParentTM);
                    matRelativeTM = matTM * inverseMat;
                }
            }

            // set the current pointer to the correct buffer position
            pbCur = (PBYTE)&rgaiAnimData[iNode*cKeys + iCurKey];

            // first write the time and dword count            
            WRITE_DWORD(pbCur, iCurTime - g_AnimFrameAge.m_dwStartFrame * iInterval); // 프레임시간을 당김
            WRITE_DWORD(pbCur, 16);

            // fix 0.00001
            for(int fi = 0; fi < 3; ++fi)
            {   
                Point3 fp = matRelativeTM.GetRow(fi);
                float afp = 1.0f;

                if(fp.x < 0.0f)
                    afp = -1.0f;
                if(abs(fp.x) < 0.0001f && abs(fp.x) >= 0.00001f)
                    fp.x = 0.0001f * afp;
                else if(abs(fp.x) < 0.00001f)
                    fp.x = 0.0f;

                afp = 1.0f;
                if(fp.x < 0.0f)
                    afp = -1.0f;
                if(abs(fp.y) < 0.0001f && abs(fp.y) >= 0.00001f)
                    fp.y = 0.0001f * afp;
                else if(abs(fp.y) < 0.00001f)
                    fp.y = 0.0f;
                
                afp = 1.0f;
                if(fp.x < 0.0f)
                    afp = -1.0f;
                if(abs(fp.z) < 0.0001f && abs(fp.z) >= 0.00001f)
                    fp.z = 0.0001f * afp;
                else if(abs(fp.z) < 0.00001f)
                    fp.z = 0.0f;

                matRelativeTM.SetRow(fi, fp);
            }

            // next write the matrix
            WRITE_MATRIX4_FROM_MATRIX3(pbCur, matRelativeTM);
        }
    }

    // if the sampling rate doesn't evenly end on the last frame, add a partial key frame

    if (bAddEndKey)
    {
        assert(((iEndTime - iStartTime) % iInterval) != 0);

        // just add the end time as a key frame
        for (iNode = 0; iNode < m_ppc.m_cNodes; iNode++)
        {

            pNode = m_rgpnNodes[iNode];

            if((g_DlgOptions.m_bSelection && !pNode->Selected()))
            {
                continue;
            }

            // set the current pointer to the correct buffer position
            pbCur = (PBYTE)&rgaiAnimData[iNode*cKeys +iCurKey];

            if( g_DlgOptions.m_bChildrenNotAttached == TRUE )
                matTM = pNode->GetObjectTM(iEndTime);	//offset이 적용된 tm
            else
                matTM = pNode->GetNodeTM(iEndTime);	//offset이 적용되지 않은 tm

            //matTM = pNode->GetNodeTM(iEndTime);   

            

            if (pNode->GetParentNode() == NULL)
            {
                matRelativeTM = matTM;
            }
            else
            {
                if( g_DlgOptions.m_bChildrenNotAttached == TRUE )
                    matParentTM = pNode->GetParentNode()->GetObjectTM(iEndTime);	//offset이 적용된 tm
                else
                    matParentTM = pNode->GetParentNode()->GetNodeTM(iEndTime);	//offset이 적용되지 않은 tm

                //matParentTM = pNode->GetParentNode()->GetNodeTM(iEndTime);
                
                if( matTM == matParentTM )
                {
                    matRelativeTM.IdentityMatrix();
                }
                else
                {
                    matRelativeTM = matTM * Inverse(matParentTM);
                }
            }

            
            
            WRITE_DWORD(pbCur, iEndTime - g_AnimFrameAge.m_dwStartFrame * iInterval); // 프레임시간을 당김
            WRITE_DWORD(pbCur, 16);

            // next write the matrix
            WRITE_MATRIX4_FROM_MATRIX3(pbCur, matRelativeTM);            
        }

        // update iCurKey for the looping key frames
        iCurKey += 1;
    }

    // add an final animation frame to allow for looping - Max assumes that the last frame lasts for an interval...  X-files do not, so specify it explicitly
    //    if looping is not desired, then this should be the final frames data instead of the first frames data
    /*
    for (iNode = 0; iNode < psc->m_cNodes; iNode++)
    {
    // set the current pointer to the correct buffer position
    pbCur = (PBYTE)&rgaiAnimData[iNode*cKeys + iCurKey];
    if (psc->m_bLoopingAnimationData)
    {
    // copy the zero'th animations data, just update the time value
    memcpy(pbCur, (PBYTE)&rgaiAnimData[iNode*cKeys + 0], sizeof(SAnimInfo));
    }
    else
    {
    // copy the previous frames data to hold for a frames worth of time
    memcpy(pbCur, (PBYTE)&rgaiAnimData[iNode*cKeys + (iCurKey-1)], sizeof(SAnimInfo));
    }

    WRITE_DWORD(pbCur, iEndTime + cTicksPerFrame - g_AnimFrameAge.m_dwStartFrame * iInterval); // 프레임시간을 당김
    }
    */

    //assert(iCurKey + 1 == cKeys);    
    assert(iCurKey == cKeys);    

    // allocate memory to send to D3DXOF lib
    cbSize = sizeof(DWORD) + sizeof(DWORD) +
        (sizeof(DWORD) //time
        + sizeof(DWORD) //nValues
        + sizeof(FLOAT)*16) // x, y, z
        * cKeys; // number of keys

    pbData = new BYTE[cbSize];
    if (pbData == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    //note : IDirectXFileData(psc->m_pAnimationSet)은 생성시 해당객체의 아이디(Ex.TID_D3DRMAnimationSet)
    //       를 주고 데이터등을 셋팅후 생성한다.
    //     : 애니메이션 네임 셋팅부분
    hr = m_pxofsave->CreateDataObject(TID_D3DRMAnimationSet,
        (LPCTSTR)strAnimName,
        NULL,
        0,
        NULL,
        &m_pAnimationSet
        );

    if (FAILED(hr))
        goto e_Exit;

    for (iNode = 0; iNode < m_ppc.m_cNodes; iNode++)
    {
        pNode = m_rgpnNodes[iNode];

        if((g_DlgOptions.m_bSelection && !pNode->Selected()))
        {
            continue;
        }

        pbCur = pbData;

        szName = g_stStrings.CreateNiceString(m_rgpnNodes[iNode]->GetName());

        if (szName == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // write the key type
        WRITE_DWORD(pbCur, 4);

        // write the number of keys
        WRITE_DWORD(pbCur, cKeys);

        rgaiAnimDataCur = &rgaiAnimData[iNode*cKeys];
        cCurrentKeys = 0;
        for (iKey = 0; iKey < cKeys; iKey++)
        {
            memcpy(pbCur, &rgaiAnimDataCur[iKey], sizeof(SAnimInfo));
            pbCur += sizeof(SAnimInfo);
            cCurrentKeys += 1;

            // if not last key, then check for start of a run of identical keys
            if (iKey < (cKeys-1))
            {
                // if equal to next, check for a run of equal matrices
                if (BEqualMatrices(rgaiAnimDataCur[iKey].rgfValues, rgaiAnimDataCur[iKey+1].rgfValues))
                {
                    // move to the next key, and skip all equal matrices
                    iKey += 1;
                    while ((iKey < (cKeys-1)) && BEqualMatrices(rgaiAnimDataCur[iKey].rgfValues, rgaiAnimDataCur[iKey+1].rgfValues))
                    {
                        iKey += 1;
                    }

                    memcpy(pbCur, &rgaiAnimDataCur[iKey], sizeof(SAnimInfo));
                    pbCur += sizeof(SAnimInfo);
                    cCurrentKeys += 1;
                }
            }
        }

        // update to current count of keys
        ((DWORD*)pbData)[1] = cCurrentKeys;

        hr = m_pxofsave->CreateDataObject(TID_D3DRMAnimation,
            NULL,
            NULL,
            0,
            NULL,
            &pAnimation
            );

        // add the data to the file
        hr = m_pxofsave->CreateDataObject(TID_D3DRMAnimationKey,
            NULL,
            NULL,
            cbSize,
            pbData,
            &pAnimationKeys
            );
        if (FAILED(hr))
            goto e_Exit;

        // add to the animation set
        hr = pAnimation->AddDataObject(pAnimationKeys);
        if (FAILED(hr))
            goto e_Exit;

        hr = pAnimation->AddDataReference(szName, NULL);

        if (FAILED(hr))
            goto e_Exit;

        hr = m_pAnimationSet->AddDataObject(pAnimation);
        if (FAILED(hr))
            goto e_Exit;

        RELEASE(pAnimation);
        RELEASE(pAnimationKeys);
    }

e_Exit:
    delete []rgaiAnimData;
    delete []pbData;
    RELEASE(pAnimation);
    RELEASE(pAnimationKeys);
#endif
    return hr;
}