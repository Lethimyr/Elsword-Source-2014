#ifndef SERDEFS_H
#define SERDEFS_H "$Id: SerDefs.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

class KSerializer;              // fwd
namespace SerializeHelper {}    // fwd

/**
 * SerializeTag enumeration
 *
 * The enumeration must fit in a UCHAR.
 **/
enum SerializeTag
{
    // numeric
    eTAG_CHAR,
    eTAG_WCHAR,// 051226. kkurrung. unsigned short 와 같음.
    eTAG_UCHAR,
    eTAG_SHORT,
    eTAG_USHORT,
    eTAG_INT,
    eTAG_DWORD,
    eTAG_INT64,
    eTAG_UINT64,
    eTAG_FLOAT,
    eTAG_DOUBLE,
    eTAG_BOOL,

    // array-like types
    eTAG_STRING,
    eTAG_WSTRING,// 051226. kkurrung. 추가.
    eTAG_ARRAY,
    eTAG_RAWBYTES,

    // STL containers (including non-standard hashed varieties)
    eTAG_PAIR,
    eTAG_VECTOR,
    eTAG_LIST,
    eTAG_DEQUE,
    eTAG_SET,
    eTAG_MULTISET,
    eTAG_MAP,
    eTAG_MULTIMAP,

    // builtin classes
    eTAG_BUFFER,
    eTAG_KEYEDSERIALIZER,

    // any user-defined class
    eTAG_USERCLASS,

    // Add here, to avoid reordering.  Or, use eTagUserClass for all
    // application-specific classes to avoid any recompiling.
    eTAG_LAST // sentinel
}; // enum SerializeTag

// The following macros simplify the declaration and definition of the
// SerializeHelper functions (ComputeTag, PutInto, GetFrom).
//
// This is one of those rare occasions where (ab)using the
// preprocessor can make things more readable.

/**
 * Define inline ComputeTag() function for given type.
 **/
#define SERIALIZE_DEFINE_TAG(typ, tag) \
    namespace SerializeHelper \
    { \
        inline SerializeTag ComputeTag(const typ&) { return tag; } \
    }

/**
 * Forward-declare PutInto() and GetFrom() for given type.
 **/
#define SERIALIZE_DECLARE_PUTGET(typ) \
        namespace SerializeHelper \
        { \
            bool PutInto(KSerializer& ser, const typ& obj); \
            bool GetFrom(KSerializer& ser, typ& obj); \
        }

/**
 * Declare PutInto() and GetFrom() for the given class as friend
 * functions of the class.  Must be put inside class declaration.
 **/
#define SERIALIZE_DECLARE_FRIEND(typ) \
        friend bool SerializeHelper::PutInto(KSerializer&, const typ&); \
        friend bool SerializeHelper::GetFrom(KSerializer&, typ&)

/**
 * Define (implement) PutInto() for given type.  Must be followed by
 * code block (i.e. this macro replaces the function header).
 *
 * Usage:
 *
 * SERIALIZE_DEFINE_PUT(Foo, obj, ser)
 * {
 *   // serialize obj (of type const Foo&) into ser (of type Serializer&)
 *   // return a bool
 * }
 **/
#define SERIALIZE_DEFINE_PUT(typ, obj, ser) \
        bool SerializeHelper::PutInto(KSerializer& ser, const typ& obj)

/**
 * Define (implement) GetFrom() for given type.  Must be followed by
 * code block (i.e. this macro replaces the function header).
 *
 * Usage:
 *
 * SERIALIZE_DEFINE_GET(Foo, obj, ser)
 * {
 *   // unserialize obj (of type Foo&) from ser (of type Serializer&)
 *   // return a bool
 * }
 **/
#define SERIALIZE_DEFINE_GET(typ, obj, ser) \
        bool SerializeHelper::GetFrom(KSerializer& ser, typ& obj)

#endif
