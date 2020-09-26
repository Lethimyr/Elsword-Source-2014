#ifndef __XFileExp_FUNC__H
#define __XFileExp_FUNC__H

#include <direct.h>
#include <d3dx9.h> 

#include "XExpUtil.h"


// Macros for saving data to memory at DWORD* pbCur (this pointer is incremented)
#define WRITE_PTCHAR(pbCur, ptchar) {TCHAR** __pptchar = (TCHAR**)pbCur; *(__pptchar++) = ptchar;\
    pbCur = (PBYTE)__pptchar;}

#define WRITE_STRING(pbCur, pstring) {TCHAR* __pCurrDestChar = (TCHAR*)pbCur; TCHAR* __pCurrOrgChar = pstring;\
    while(NULL != *__pCurrOrgChar) { *(__pCurrDestChar++) = *(__pCurrOrgChar++); }\
    *(__pCurrDestChar++) = _T('\0'); pbCur = __pCurrDestChar;}\

#define WRITE_WORD(pbCur, word) {WORD* __pword = (WORD*)pbCur; *(__pword++) = word;\
    pbCur = (PBYTE)__pword;}

#define WRITE_DWORD(pbCur, dword) {DWORD* __pdword = (DWORD*)pbCur; *(__pdword++) = dword;\
    pbCur = (PBYTE)__pdword;}

#define WRITE_FLOAT(pbCur, _float_value) {float* __pfloat = (float*)pbCur; float _float = (float)_float_value; \
    NAN_FLOAT_TEST(&_float); \
    *(__pfloat++) = _float; pbCur = (PBYTE)__pfloat;}

//###danhoro $NOTE switched MAX Z-up RHS to D3D Z-front LHS by swapping element 1 and 2
#define WRITE_POINT3(pbCur, _point3) {Point3 _temp = (Point3)_point3; float __tempVal;\
    __tempVal = _temp[0]; WRITE_FLOAT(pbCur, __tempVal);\
    __tempVal = _temp[2]; WRITE_FLOAT(pbCur, __tempVal);\
    __tempVal = _temp[1]; WRITE_FLOAT(pbCur, __tempVal);}

#define WRITE_COLOR(pbCur, _color) {D3DXCOLOR _temp = (D3DXCOLOR)_color; float __tempVal;\
    __tempVal = _temp.r; WRITE_FLOAT(pbCur, __tempVal);\
    __tempVal = _temp.g; WRITE_FLOAT(pbCur, __tempVal);\
    __tempVal = _temp.b; WRITE_FLOAT(pbCur, __tempVal);\
    __tempVal = _temp.a; WRITE_FLOAT(pbCur, __tempVal);}

//###danhoro $NOTE switched MAX Z-up RHS to D3D Z-front LHS by swapping row 1 and 2
#define WRITE_MATRIX4_FROM_MATRIX3(pbCur, _matrix3) {Point3 __tempRow = ((Matrix3)_matrix3).GetRow(0);\
    WRITE_POINT3(pbCur, __tempRow); WRITE_FLOAT(pbCur, 0);\
    __tempRow = _matrix3.GetRow(2); WRITE_POINT3(pbCur, __tempRow); WRITE_FLOAT(pbCur, 0);\
    __tempRow = _matrix3.GetRow(1); WRITE_POINT3(pbCur, __tempRow); WRITE_FLOAT(pbCur, 0);\
    __tempRow = _matrix3.GetRow(3); WRITE_POINT3(pbCur, __tempRow); WRITE_FLOAT(pbCur, 1);}

#define WRITE_MATRIX(pbCur, mat) { *(D3DXMATRIX*)pbCur = mat;\
    pbCur = (PBYTE)pbCur + sizeof(D3DXMATRIX);}


#endif