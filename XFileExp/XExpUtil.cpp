#include "XExpUtil.h"


bool gbNan_found= false;

void NAN_FLOAT_TEST(float *fValue)
{

    if(0 != _isnan(*fValue) || *fValue != *fValue)
    {
        if(gbNan_found == false)
        {
            MessageBox(NULL, "Exporter found a NAN float value and has set it to 0.0f.  The resulting X-File may not be accurate.", "ERROR: NAN FLOAT", MB_OK|MB_ICONERROR);
        }

        gbNan_found= true;

        *fValue= 0.0f;
    }
}


TCHAR* FixupName(TCHAR* name)
{
    static char buffer[256];
    TCHAR* cPtr;

    _tcscpy(buffer, name);
    cPtr = buffer;

    while(*cPtr) 
    {
        if (*cPtr == '"')
            *cPtr = 39;
        else if (*cPtr <= 31)
            *cPtr = _T('_');
        else if (*cPtr == ' ')
            *cPtr = _T('_');
        cPtr++;
    }

    return buffer;
}

void CommaScan(TCHAR* buf)
{
    for(; *buf; buf++) if (*buf == ',') *buf = '.';
}


void ConvertDXPoision(Point3 &v)
{
    Point3 tempP = v;

    v.y = tempP.z;
    v.z = tempP.y;
}

TSTR Format(int value)
{
    TCHAR buf[50];

    sprintf(buf, _T("%d"), value);
    return buf;
}


TSTR Format(float value)
{
    TCHAR buf[32];

    sprintf(buf, "%.4f", value);
    CommaScan(buf);
    return TSTR(buf);
}

TSTR Format(Point3 value)
{
    TCHAR buf[64];

    sprintf(buf, "%.4f %.4f %.4f", value.x, value.y, value.z);

    CommaScan(buf);
    return buf;
}

TSTR Format(Color value)
{
    TCHAR buf[64];

    sprintf(buf, "%.4f %.4f %.4f", value.r, value.g, value.b);

    CommaScan(buf);
    return buf;
}

TSTR Format(AngAxis value)
{
    TCHAR buf[64];

    sprintf(buf, "%.4f %.4f %.4f %.4f", value.axis.x, value.axis.y, value.axis.z, value.angle);

    CommaScan(buf);
    return buf;
}


TSTR Format(Quat value)
{
    // A Quat is converted to an AngAxis before output.

    Point3 axis;
    float angle;
    AngAxisFromQ(value, &angle, axis);

    return Format(AngAxis(axis, angle));
}

TSTR Format(ScaleValue value)
{
    TCHAR buf[280];

    sprintf(buf, "%s %s", Format(value.s), Format(value.q));
    CommaScan(buf);
    return buf;
}

void RHMatToLHMat ( Matrix3 &matRH, Matrix3 &matLH )
{
    //	EDITED : matRH 성분을 분해.
    //
    AffineParts ap;
    decomp_affine ( matRH, &ap );

    //	Note : Position 성분 - y와 z를 뒤바꾼다.
    //
    Point3 Trans;
    Trans.x = ap.t.x;
    Trans.y = ap.t.z;
    Trans.z = ap.t.y;

    ap.t.x = Trans.x;
    ap.t.y = Trans.y;
    ap.t.z = Trans.z;

    //	Note : Rotation 성분 - y와 z를 뒤바꾼다.
    //
    Quat Q;
    Q.x = ap.q.x;
    Q.y = ap.q.z;
    Q.z = ap.q.y;
    Q.w = -ap.q.w;
    ap.q = Q;

    Q.x = ap.u.x;
    Q.y = ap.u.z;
    Q.z = ap.u.y;
    Q.w = -ap.u.w;
    ap.u = Q;

    Point3 Scale;
    Scale.x = ap.k.x;
    Scale.y = ap.k.z;
    Scale.z = ap.k.y;

    ap.k.x = Scale.x;
    ap.k.y = Scale.y;
    ap.k.z = Scale.z;


    //	Note : 트렌스폼 성분을 다시 합성.
    //
    Matrix3 srtm, rtm, ptm, stm, ftm;

    ptm.IdentityMatrix ();
    ptm.SetTrans ( ap.t );

    ap.q.MakeMatrix ( rtm );
    ap.u.MakeMatrix ( srtm );

    stm = ScaleMatrix ( ap.k );
    ftm = ScaleMatrix ( Point3(ap.f,ap.f,ap.f) );

    matLH = Inverse(srtm) * stm * srtm * rtm * ftm * ptm;
}

// used by 3DS progress bar
DWORD WINAPI ProgressFn(LPVOID arg)
{
    return(0);
}

BOOL IsExportableMesh(INode* pNode, Object** pObj)
{
    ULONG superClassID;
    Object *pObjBase;

    assert(pNode);

    pObjBase = pNode->GetObjectRef();
    if (pObjBase == NULL)
        return FALSE;

    if(pObj)*pObj= pObjBase;

    superClassID = pObjBase->SuperClassID();
    //find out if mesh is renderable

    if( !pNode->Renderable() || pNode->IsNodeHidden())
    {
        return FALSE;
    }

    BOOL bFoundGeomObject = FALSE;
    //find out if mesh is renderable (more)
    switch(superClassID)
    {
    case GEN_DERIVOB_CLASS_ID:

        do
        {
            pObjBase = ((IDerivedObject*)pObjBase)->GetObjRef();
            superClassID = pObjBase->SuperClassID();
        }
        while( superClassID == GEN_DERIVOB_CLASS_ID );

        switch(superClassID)
        {
        case GEOMOBJECT_CLASS_ID:
            bFoundGeomObject = TRUE;
            break;
        }
        break;

    case GEOMOBJECT_CLASS_ID:
        bFoundGeomObject = TRUE;

    default:
        break;
    }

    if(pObj)*pObj= pObjBase;

    return bFoundGeomObject || pObjBase->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0));
}

TriObject *GetTriObjectFromObjRef( Object* pObj,  BOOL *pbDeleteIt ) 
{


    TriObject *pTri;

    assert(pObj != NULL);
    assert(pbDeleteIt != NULL);

    *pbDeleteIt = FALSE;

    //###danhoro $NOTE: meshes were not properly evaluated
    pObj= pObj->Eval(0).obj;

    assert(pObj != NULL);

    if (pObj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
    { 
        pTri = (TriObject *) pObj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));

        // Note that the TriObject should only be deleted
        // if the pointer to it is not equal to the object
        // pointer that called ConvertToType()
        if (pObj != pTri) 
            *pbDeleteIt = TRUE;

        return pTri;
    } 
    else 
    {
        return NULL;
    }
}

BOOL BFloatsEqual( float f1, float f2 )
{
    // first do a bitwise compare
    if ((*(DWORD*)&f1) == (*(DWORD*)&f2))
        return TRUE;

    // next do an epsilon compare
    float fDiff = (f1 - f2);
    return (fDiff <= 1e-6f) && (fDiff >= -1e-6f);
}

BOOL BEqualMatrices(float *rgfMat1, float *rgfMat2)
{
    DWORD iFloat;

    for (iFloat = 0; iFloat < 16; iFloat++)
    {
        if (!BFloatsEqual(rgfMat1[iFloat], rgfMat2[iFloat]))
            return FALSE;
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------

CXFrameSort::CXFrameSort()
{
    m_pRoot = NULL;
    m_bAttach = FALSE;
    m_pNode = NULL;
}

CXFrameSort::~CXFrameSort()
{
    CXFrameSort *node;

    for(int i=0; i<m_pChild.size(); ++i)
    {
        node = m_pChild[i];
        if(node)
            delete node;
    }

}

CXFrameSort *CXFrameSort::GetNode(TCHAR *szName, int *aIndex)
{    
    CXFrameSort *resultNode = NULL;
    int resultInx = 0;

    if(_tcscmp(szName, m_strName) == 0)
    {
        return this;
    }
    else
    {
        for(int i=0; i<m_pChild.size(); ++i)
        {
            resultNode = GetNode(szName, aIndex);
            if(resultNode)
            {
                *aIndex = i;
                return resultNode;
            }
        }

        return NULL;
    }
}

void CXFrameSort::AddNode(LPDIRECTXFILEDATA pNode, TCHAR *szName)
{
    CXFrameSort *node = new CXFrameSort();

    
    _tcscpy(node->m_strName, szName);
    node->m_pNode = pNode;

    m_pChild.push_back(node);
}
