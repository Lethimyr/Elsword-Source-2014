#ifndef __XFileExp_UTIL__H
#define __XFileExp_UTIL__H

#include "XFileExp.h"

/** @function 	: 
	@brief 		: 
*/
void NAN_FLOAT_TEST(float *fValue);

/** @function 	: 
	@brief 		: 
*/
TCHAR*  FixupName(TCHAR* name);

/** @function 	: 
	@brief 		: 
*/
void    CommaScan(TCHAR* buf);

/** @function 	: 
	@brief 		: 
*/
TSTR	Format(int value);
TSTR	Format(float value);
TSTR	Format(Color value);
TSTR	Format(Point3 value); 
TSTR	Format(AngAxis value); 
TSTR	Format(Quat value);
TSTR	Format(ScaleValue value);

/** @function 	: 
	@brief 		: 
*/
void    RHMatToLHMat ( Matrix3 &matRH, Matrix3 &matLH );

/** @function 	: 
	@brief 		: 
*/
DWORD WINAPI ProgressFn(LPVOID arg);

/** @function 	: 
	@brief 		: 
*/
void ConvertDXPoision(Point3 &v);

/** @function 	: 
	@brief 		: 
*/
BOOL IsExportableMesh(INode* pNode, Object** pObj);

/** @function 	: 
	@brief 		: 
*/
TriObject *GetTriObjectFromObjRef( Object* pObj,  BOOL *pbDeleteIt ) ;

/** @function 	: 
	@brief 		: 
*/
BOOL BFloatsEqual( float f1, float f2 );

/** @function 	: 
	@brief 		: 
*/
BOOL BEqualMatrices(float *rgfMat1, float *rgfMat2);


extern bool gbNan_found;

#if 0 // Option Save
struct SOptionReg
{
    BOOL m_bOptimize;
    BOOL m_bSmoothNormal;
    BOOL m_xFormat;
    BOOL m_bObjectByYExtension;
    BOOL m_bTextureFullPath;
};
#endif

/** @struct : SDialogOptions
	@brief  : 
*/
struct SDialogOptions
{
    Interface       *m_pInterface;
    BOOL            m_bSelection;
    const TCHAR     *m_pFileName;
    BOOL            m_bFileTypeX;
    BOOL            m_bProceedWithExport;
    BOOL            m_bOptimize;
    BOOL            m_bSmoothNormal;
    BOOL            m_bEmptyFrameAttach;
    BOOL            m_bConvertKOGMesh;

    DWORD           m_cMaxBonesPerVertex;
    DWORD           m_cMaxBonesPerFace;

    DXFILEFORMAT    m_xFormat;
    BOOL            m_bObjectByYExtension;
    BOOL            m_bChildrenNotAttached;

    BOOL			m_bTextureFullPath;

    BOOL            m_bAddSkinWeight;
    BOOL            m_bSaveAnimationData;        
    DWORD           m_iAnimSamplingRate;
    TCHAR			m_strAnimName[MAX_PATH];
	TCHAR			m_strOrgAnimName[MAX_PATH];


    DWORD           m_nTotalVertex1, m_nTotalVertex2;
    DWORD           m_nTotalFace1, m_nTotalFace2;
    DWORD           m_nTotalNode;
    DWORD           m_iCurNode;

	BOOL			m_bMultiFile;		/// 배경 출력용, 여러 개의 파일을 한 번에 추출
};

enum 
{ 
    bmtex_clipu,bmtex_clipv,bmtex_clipw,bmtex_cliph,
    bmtex_jitter,bmtex_usejitter,
    bmtex_apply,bmtex_crop_place,
    bmtex_filtering,
    bmtex_monooutput,
    bmtex_rgboutput,
    bmtex_alphasource,
    bmtex_premultalpha,
    bmtex_bitmap,
    bmtex_coords,	 // access for UVW mapping
    bmtex_output,	 //output window
    bmtex_filename   // bitmap filename virtual parameter, JBW 2/23/99
};


class CXFrameSort
{
public:
    CXFrameSort();
    ~CXFrameSort();

// Attribute    
public:
    CXFrameSort *m_pRoot;
    std::vector<CXFrameSort *> m_pChild;

    BOOL    m_bAttach;          // 미리 모든 Frame을 순회하여 현재 Frame이 attach될 Frame인지 체크한다.
    TCHAR   m_strName[256];
    LPDIRECTXFILEDATA m_pNode;

// Method
public:
    CXFrameSort *GetNode(TCHAR *szName, int *aIndex);
    void AddNode(LPDIRECTXFILEDATA pNode, TCHAR *szName);
};

class FindTextureFilename : public NameEnumCallback
{
public:
    FindTextureFilename()
        :m_szTextureName(NULL) {}

        virtual void RecordName(TCHAR *szName)
        {
            m_szTextureName = szName;
        }

        TCHAR *m_szTextureName;    
};

const int x_cbStringBufferMax = 4088;
struct SStringBlock
{
    SStringBlock()
        :m_psbNext(NULL), m_cbData(0) {}
        ~SStringBlock()
        {
            delete m_psbNext;
        }

        SStringBlock				*m_psbNext;
        DWORD						m_cbData;

        TCHAR						szData[x_cbStringBufferMax];
};

class CStringTable
{
public:
    CStringTable()
        :m_psbHead(NULL) {}

        ~CStringTable()
        {
            delete m_psbHead;
        }

        // allocate a string out of the data blocks to be free'd later, and make it a valid
        //   x-file name at the same time
        TCHAR *CreateNiceString(TCHAR *szString)
        {
            TCHAR* szNewString = NULL;
            BOOL bFirstCharIsDigit;
            DWORD cbLength;
            SStringBlock *psbNew;

            if (szString == NULL)
                return NULL;

            cbLength = _tcslen(szString) + 1;

            bFirstCharIsDigit = _istdigit(*szString);
            if (bFirstCharIsDigit)
            {
                cbLength += 1;
            }

            // if no string blocks or the current doesn't have enough space, then allocate one
            if ((m_psbHead == NULL) || ((x_cbStringBufferMax - m_psbHead->m_cbData) < cbLength))
            {
                psbNew = new SStringBlock();
                if (psbNew == NULL)
                    return NULL;

                psbNew->m_psbNext = m_psbHead;
                m_psbHead = psbNew;
            }

            // allocate a string out of the data block
            szNewString = m_psbHead->szData + m_psbHead->m_cbData;
            m_psbHead->m_cbData += cbLength;

            // deal with the fact that the string can't start with digits
            *szNewString = _T('\0');
            if( bFirstCharIsDigit ) 
            {
                _tcscat(szNewString, _T("_"));
            }

            _tcscat(szNewString, szString);

            TCHAR* pchCur = szNewString;
            while( NULL != *pchCur )
            {
                if( *pchCur != _T('_') && !_istalnum(*pchCur) )
                {
                    *pchCur = _T('_');
                }
                pchCur++;
            }
            return szNewString;
        }

        // Allocate a new string with '\\' in place of '\' characters
        TCHAR* CreateNiceFilename(TCHAR *szString)
        {
            TCHAR* szNewString = NULL;
            DWORD cbNameLength;
            DWORD cbLength;
            TCHAR* pchCur;
            TCHAR* pchOrig;
            SStringBlock *psbNew;

            if( NULL == szString )
            {
                return NULL;
            }

            cbNameLength = _tcslen(szString);
            cbLength = cbNameLength*2 + 1;


            // if no string blocks or the current doesn't have enough space, then allocate one
            if ((m_psbHead == NULL) || ((x_cbStringBufferMax - m_psbHead->m_cbData) < cbLength))
            {
                psbNew = new SStringBlock();
                if (psbNew == NULL)
                    return NULL;

                psbNew->m_psbNext = m_psbHead;
                m_psbHead = psbNew;
            }

            // allocate a string out of the data block
            szNewString = m_psbHead->szData + m_psbHead->m_cbData;
            m_psbHead->m_cbData += cbLength;

            pchCur = szNewString;
            pchOrig = szString;
            while (NULL != *pchOrig)
            {
                if( _T('\\') == *pchOrig )
                {
                    *(pchCur++) = _T('\\');
                    *(pchCur++) = _T('\\');
                }
                else
                {
                    *(pchCur++) = *pchOrig;
                }
                pchOrig++;
            }
            *pchCur = _T('\0');

            return szNewString;
        }

        // Allocate a new string without fiddling with the '\' characters
        TCHAR* CreateNormalFilename(TCHAR *szString)
        {
            TCHAR* szNewString = NULL;
            DWORD cbNameLength;
            DWORD cbLength;
            SStringBlock *psbNew;

            if( NULL == szString )
            {
                return NULL;
            }

            cbNameLength = _tcslen(szString);
            cbLength = cbNameLength + 1;


            // if no string blocks or the current doesn't have enough space, then allocate one
            if ((m_psbHead == NULL) || ((x_cbStringBufferMax - m_psbHead->m_cbData) < cbLength))
            {
                psbNew = new SStringBlock();
                if (psbNew == NULL)
                    return NULL;

                psbNew->m_psbNext = m_psbHead;
                m_psbHead = psbNew;
            }

            // allocate a string out of the data block
            szNewString = m_psbHead->szData + m_psbHead->m_cbData;
            m_psbHead->m_cbData += cbLength;

            memcpy(szNewString, szString, cbLength);

            return szNewString;
        }
private:
    SStringBlock *m_psbHead;
};

#endif