// NUMERICAL DESIGN LIMITED PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Numerical Design Limited and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2004 Numerical Design Limited.
//      All Rights Reserved.
//
// Numerical Design Limited, Chapel Hill, North Carolina 27514
// http://www.ndl.com

#ifndef NIRTTI_H
#define NIRTTI_H

//#include "NiMainLibType.h"    // florist

// run-time type information

//class NIMAIN_ENtry NiRTTI
class NiRTTI    // florist
{
public:
    NiRTTI (const wchar_t* pcName, const NiRTTI* pkBaseRTTI)
    { m_pcName = pcName; m_pkBaseRTTI = pkBaseRTTI; }   // florist

    const wchar_t* GetName() const {return m_pcName;}
    const NiRTTI* GetBaseRTTI() const {return m_pkBaseRTTI;}

protected:
    const wchar_t* m_pcName;
    const NiRTTI* m_pkBaseRTTI;
};

// insert in root class declaration
#define NiDeclareRootRTTI(classname) \
    public: \
    static const NiRTTI m_RTTI; \
    virtual const NiRTTI* GetRTTI() const {return &m_RTTI;} \
    static bool IsExactKindOf(const NiRTTI* pkRTTI, \
    const classname* pkObject) \
    { \
        if (!pkObject) \
        { \
            return false; \
        } \
        return pkObject->IsExactKindOf(pkRTTI); \
    } \
    bool IsExactKindOf(const NiRTTI* pkRTTI) const \
    { \
        return (GetRTTI() == pkRTTI); \
    } \
    static bool IsKindOf(const NiRTTI* pkRTTI, \
                         const classname* pkObject) \
    { \
        if (!pkObject) \
        { \
            return false; \
        } \
        return pkObject->IsKindOf(pkRTTI); \
    } \
    bool IsKindOf(const NiRTTI* pkRTTI) const \
    { \
        const NiRTTI* pkTmp = GetRTTI(); \
        while (pkTmp) \
        { \
            if (pkTmp == pkRTTI) \
            { \
                return true; \
            } \
            pkTmp = pkTmp->GetBaseRTTI(); \
        } \
        return false; \
    } \
    static void* DynamicCast(const NiRTTI* pkRTTI, \
                             const classname* pkObject) \
    { \
        if (!pkObject) \
        { \
            return false; \
        } \
        return pkObject->DynamicCast(pkRTTI); \
    } \
    void* DynamicCast(const NiRTTI* pkRTTI) const \
    { \
        return (IsKindOf(pkRTTI) ? (void*) this : 0 ); \
    }

// insert in class declaration
#define NiDeclareRTTI \
    public: \
    static const NiRTTI m_RTTI; \
    virtual const NiRTTI* GetRTTI() const {return &m_RTTI;}

// insert in root class source file
#define NiImplementRootRTTI(rootclassname) \
    const NiRTTI rootclassname::m_RTTI(L#rootclassname, 0)

// insert in class source file
#define NiImplementRTTI(classname, baseclassname) \
    const NiRTTI classname::m_RTTI(L#classname, &baseclassname::m_RTTI)

// macros for run-time type testing
#define NiIsExactKindOf(classname, pkObject) \
    classname::IsExactKindOf(&classname::m_RTTI, pkObject)

#define NiIsKindOf(classname, pkObject) \
    classname::IsKindOf(&classname::m_RTTI, pkObject)

// macro for run-time type casting
#define NiStaticCast(classname, pkObject) \
    ((classname*) pkObject)

#define NiDynamicCast(classname, pkObject) \
    ((classname*) classname::DynamicCast(&classname::m_RTTI, pkObject))

#endif
