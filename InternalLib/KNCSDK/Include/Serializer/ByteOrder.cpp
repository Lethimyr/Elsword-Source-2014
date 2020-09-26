static const char* cvsid = "$Id: ByteOrder.cpp,v 1.2 2003/01/14 01:55:38 cvs Exp $";

#include "ByteOrder.h"

#define TEMP_SWAP(a,b,tmp) tmp = a; a = b; b = tmp

/**
 * Returns physically converted 16-bit integer
 **/
USHORT ByteOrder::SwapInt16(USHORT x)
{
    // get the individual bytes
    UCHAR b0 = static_cast<UCHAR>(x & 0xFF);
    UCHAR b1 = static_cast<UCHAR>((x >> 8) & 0xFF);
    // put them back in reverse order
    return static_cast<USHORT>((b0 << 8) | b1);
}

/**
 * Returns physically converted 32-bit integer
 **/
DWORD ByteOrder::SwapInt32(DWORD x)
{
    // get the individual bytes
    UCHAR b0 = static_cast<UCHAR>(x & 0xFF);
    UCHAR b1 = static_cast<UCHAR>((x >> 8) & 0xFF);
    UCHAR b2 = static_cast<UCHAR>((x >> 16) & 0xFF);
    UCHAR b3 = static_cast<UCHAR>((x >> 24) & 0xFF);
    // put them back in reverse order
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}


/**
* Returns physically converted 64-bit integer
**/
UINT64 ByteOrder::SwapInt64(UINT64 x)
{
    // use union hack
    union
    {
        UINT64 val;
        UCHAR bytes[8];
    } u;
    u.val = x;
    // 0 1 2 3 4 5 6 7 -> 7 6 5 4 3 2 1 0
    UCHAR tmp;
    TEMP_SWAP(u.bytes[0], u.bytes[7], tmp);
    TEMP_SWAP(u.bytes[1], u.bytes[6], tmp);
    TEMP_SWAP(u.bytes[2], u.bytes[5], tmp);
    TEMP_SWAP(u.bytes[3], u.bytes[4], tmp);
    return u.val;
}


/**
 * Returns physically converted 32-bit float
 **/
float ByteOrder::SwapFloat32(float x)
{
    // use union hack
    union
    {
        float val;
        UCHAR bytes[4];
    } u;
    u.val = x;
    // 0 1 2 3 -> 3 2 1 0
    UCHAR tmp;
    TEMP_SWAP(u.bytes[0], u.bytes[3], tmp);
    TEMP_SWAP(u.bytes[1], u.bytes[2], tmp);
    return u.val;
}

/**
 * Returns physically converted 64-bit float
 **/
double ByteOrder::SwapFloat64(double x)
{
    // use union hack
    union
    {
        double val;
        UCHAR bytes[8];
    } u;
    u.val = x;
    // 0 1 2 3 4 5 6 7 -> 7 6 5 4 3 2 1 0
    UCHAR tmp;
    TEMP_SWAP(u.bytes[0], u.bytes[7], tmp);
    TEMP_SWAP(u.bytes[1], u.bytes[6], tmp);
    TEMP_SWAP(u.bytes[2], u.bytes[5], tmp);
    TEMP_SWAP(u.bytes[3], u.bytes[4], tmp);
    return u.val;
}
