#ifndef BYTEORDER_H
#define BYTEORDER_H "$Id: ByteOrder.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

#include "BaseTypes.h"

namespace ByteOrder
{
    USHORT SwapInt16(USHORT x);
    DWORD  SwapInt32(DWORD x);
    UINT64 SwapInt64(UINT64 x);
    float  SwapFloat32(float x);
    double SwapFloat64(double x);
}

// byteorder definitions
#define BYTEORDER_BIG       'B'
#define BYTEORDER_LITTLE    'L'

// win32에서만 사용 될 경우 byte order 변경 하지 않는다
#define BYTEORDER_HOST      BYTEORDER_LITTLE
//#define BYTEORDER_NET       BYTEORDER_LITTLE
#define BYTEORDER_NET       BYTEORDER_BIG


#if( BYTEORDER_HOST == BYTEORDER_NET )
#   define HTON_I16(x) (x)
#   define HTON_I32(x) (x)
#   define HTON_I64(x) (x)
#   define HTON_F32(x) (x)
#   define HTON_F64(x) (x)
#else
#   define HTON_I16(x) (ByteOrder::SwapInt16(static_cast<USHORT>(x)))
#   define HTON_I32(x) (ByteOrder::SwapInt32(static_cast<DWORD>(x)))
#   define HTON_I64(x) (ByteOrder::SwapInt64(static_cast<UINT64>(x)))
#   define HTON_F32(x) (ByteOrder::SwapFloat32(x))
#   define HTON_F64(x) (ByteOrder::SwapFloat64(x))
#endif

#define NTOH_I16(x) HTON_I16(x)
#define NTOH_I32(x) HTON_I32(x)
#define NTOH_I64(x) HTON_I64(x)
#define NTOH_F32(x) HTON_F32(x)
#define NTOH_F64(x) HTON_F64(x)

#endif
