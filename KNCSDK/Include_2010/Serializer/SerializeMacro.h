#pragma once

#include "Serializer/SerPair.h"
#include "Serializer/SerList.h"
#include "Serializer/SerVector.h"
#include "Serializer/SerDeque.h"
#include "Serializer/SerMap.h"
#include "Serializer/SerSet.h"
#include "Serializer/SerBuffer.h"

#define DECL_DATA(name)     struct name; \
                            SERIALIZE_DEFINE_TAG(name, eTAG_USERCLASS); \
                            SERIALIZE_DECLARE_PUTGET(name); \
                            struct name
#define DECL_PACKET(id)     DECL_DATA( K##id )
#define COPYCON_ASSIGNOP( type, t )     type( const type &t ) { *this = t; } \
                                        type& operator=( const type& t )
#define CON_COPYCON_ASSIGNOP( type, t ) type() {} \
                                        type(const type& t) { *this = t; } \
                                        type& operator=(const type& t)

#define PUT( x )            ks.Put( obj.x )
#define GET( x )            ks.Get( obj.x )
#define PUTARRAY( x, size ) ks.PutArray( obj.x, size )
#define GETARRAY( x, size ) ks.GetArray( obj.x, dwSize = size ) && (dwSize == size)