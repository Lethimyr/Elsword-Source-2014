#include <boost/test/unit_test.hpp>

#include "Serializer/Serbuffer.h"
#include "Serializer/Serializer.h"
#include "Serializer/SerPair.h"
#include "Serializer/SerList.h"
#include "Serializer/SerVector.h"
#include "Serializer/SerDeque.h"
#include "Serializer/SerMap.h"
#include "Serializer/SerSet.h"

#include <iostream>

#include "Serializer/KeyedSerializer.h"

//#include "PacketHeader.h"

bool g_bUseTagging = true;


void DumpBuffer(const KSerBuffer& buf)
{
    DWORD len = buf.GetLength();
    printf("Buffer: size=%u", len);
    if (len > 0)
    {
        printf("  [ ");
        const UCHAR* pBuf = (const UCHAR*) buf.GetData();
        for (DWORD i = 0; i < len; ++i)
            printf("%02x ", pBuf[i]);
        printf("]");
    }
    printf("\n");
}

// test용 함수
// 주어진 값을 buffer에 serialize하고 unserialize 수행, 결과 비교
template <typename T>
void TestSerialize(const T& val)
{
    KSerializer     s;
    KSerBuffer      buf;

    // write
    BOOST_CHECK( s.BeginWriting(&buf, g_bUseTagging) );
    BOOST_CHECK( s.Put(val) );
    BOOST_CHECK( s.EndWriting() ); 

    DumpBuffer(buf);

    // read
    T val_read;
    BOOST_CHECK( s.BeginReading(&buf, g_bUseTagging) );
    BOOST_CHECK( s.Get(val_read) );
    BOOST_CHECK( s.EndReading() );

    // nothing left to read  
    BOOST_CHECK( buf.GetReadLength() == 0 );

    // we read what we wrote 
    BOOST_CHECK( val == val_read );
}

void TestSerializeString(const std::string& str)
{
    printf("TestString: '%s'\n", str.c_str());
    TestSerialize(str);
}

void TestSerializeCharPtr(const char* str)
{
    printf("TestCharPtr: '%s'\n", str);

    KSerializer s;
    KSerBuffer buf;

    // write
    BOOST_CHECK( s.BeginWriting(&buf, g_bUseTagging) );
    BOOST_CHECK( s.Put(str) );
    BOOST_CHECK( s.EndWriting() );

    DumpBuffer(buf);


    // read (into C++ string)
    std::string str_read;
    BOOST_CHECK( s.BeginReading(&buf, g_bUseTagging) );
    BOOST_CHECK( s.Get(str_read) );
    BOOST_CHECK( s.EndReading() );

    // nothing left to read
    BOOST_CHECK( buf.GetReadLength() == 0 );

    // we read what we wrote
    BOOST_CHECK( std::string(str) == str_read );
}
 
//{{ Begin 051226. kkurrung.
void TestSerializeString(const std::wstring& str)
{
    wprintf(L"TestString: '%s'\n", str.c_str());
    TestSerialize(str);
}

void TestSerializeCharPtr(const wchar_t* str)
{
    printf("TestCharPtr: '%s'\n", str);

    KSerializer s;
    KSerBuffer buf;

    // write
    BOOST_CHECK( s.BeginWriting(&buf, g_bUseTagging) );
    BOOST_CHECK( s.Put(str) );
    BOOST_CHECK( s.EndWriting() );

    DumpBuffer(buf);


    // read (into C++ string)
    std::wstring str_read;
    BOOST_CHECK( s.BeginReading(&buf, g_bUseTagging) );
    BOOST_CHECK( s.Get(str_read) );
    BOOST_CHECK( s.EndReading() );

    // nothing left to read
    BOOST_CHECK( buf.GetReadLength() == 0 );

    // we read what we wrote
    std::wstring tmpStr(str);
    BOOST_CHECK( tmpStr == str_read );
    int k = tmpStr.size();
    int j = str_read.size();
    printf( "%d == %d\n", k, j );

}
//}} 051226. End kkurrung.



template <typename T>
void TestSerializeArray(const T* ary, DWORD numItems)
{
    printf("TestArray: numItems=%u\n", numItems);

    KSerializer s;
    KSerBuffer buf;

    // write
    BOOST_CHECK( s.BeginWriting(&buf, g_bUseTagging) );
    BOOST_CHECK( s.PutArray(ary, numItems) );
    BOOST_CHECK( s.EndWriting() );

    DumpBuffer(buf);

    // read
    T* ary_read = new T[numItems];
    DWORD numItems_read = numItems; // in/out
    BOOST_CHECK( s.BeginReading(&buf, g_bUseTagging) );
    BOOST_CHECK( s.GetArray(ary_read, numItems_read) );
    BOOST_CHECK( s.EndReading() );

    // nothing left to read
    BOOST_CHECK( buf.GetReadLength() == 0 );

    // we read what we wrote
    BOOST_CHECK( numItems == numItems_read );
    for (DWORD i = 0; i < numItems; ++i)
        BOOST_CHECK( ary[i] == ary_read[i] );

    // cleanup
    delete [] ary_read;
}

void TestSerializeRaw(const void* data, DWORD len)
{
    printf("TestRaw: len=%u\n", len);

    KSerializer s;
    KSerBuffer buf;

    // write
    BOOST_CHECK( s.BeginWriting(&buf, g_bUseTagging) );
    BOOST_CHECK( s.PutRaw(data, len) );
    BOOST_CHECK( s.EndWriting() );

    DumpBuffer(buf);

    // read
    UCHAR* data_read = new UCHAR[len];
    BOOST_CHECK( s.BeginReading(&buf, g_bUseTagging) );
    BOOST_CHECK( s.GetRaw(data_read, len) );
    BOOST_CHECK( s.EndReading() );

    // nothing left to read
    BOOST_CHECK( buf.GetReadLength() == 0 );

    // we read what we wrote
    const UCHAR* data_tmp = (const UCHAR*) data;
    for (DWORD i = 0; i < len; ++i)
        BOOST_CHECK( data_tmp[i] == data_read[i] );

    // cleanup
    delete [] data_read;
}

void TestNetSerialize()
{
    /*
    KNetSerializer kns;
    KSerBuffer  kbuff;
    KPacketHeader   khead;

    khead.m_usOpCode = 255;

    BOOST_CHECK( kns.BeginWriting(&kbuff) );
    BOOST_CHECK( kns.Put(khead) );
    BOOST_CHECK( kns.EndWriting() );

    DumpBuffer( kbuff );

    KPacketHeader   kheader;
    BOOST_CHECK( kns.BeginReading(&kbuff) );
    BOOST_CHECK( kns.Get(kheader) );
    BOOST_CHECK( kns.EndReading() );
    /**/
} 

struct Header; // fwd
SERIALIZE_DEFINE_TAG(Header, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(Header);

struct Header
{
    DWORD                   m_parent;

    bool Equals(const Header& other) const {
        return (m_parent == other.m_parent);
    }

    SERIALIZE_DECLARE_FRIEND(Header);
};

SERIALIZE_DEFINE_PUT(Header, obj, serializer)
{
    return serializer.Put(obj.m_parent);
}

SERIALIZE_DEFINE_GET(Header, obj, serializer)
{
    return serializer.Get(obj.m_parent);
}

bool operator==(const Header& lhs, const Header& rhs)
{
    return lhs.Equals(rhs);
}

bool operator!=(const Header& lhs, const Header& rhs)
{
    return !lhs.Equals(rhs);
}



class Foo; // fwd
SERIALIZE_DEFINE_TAG(Foo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(Foo);

class Foo
{
public:
    Foo() { }
    ~Foo() { }

    void Init(DWORD x, const std::string& s, std::vector<Header> lf) {
        m_x = x;
        m_s = s;
        m_lf = lf;
    }

    bool Equals(const Foo& other) const {
        return (m_x == other.m_x) && (m_s == other.m_s) && (m_lf == other.m_lf);
    }

private:
    DWORD                   m_x;
    std::string             m_s;
    std::vector<Header>     m_lf;

    SERIALIZE_DECLARE_FRIEND(Foo);
};

bool operator==(const Foo& lhs, const Foo& rhs)
{
    return lhs.Equals(rhs);
}

bool operator!=(const Foo& lhs, const Foo& rhs)
{
    return !lhs.Equals(rhs);
}

SERIALIZE_DEFINE_PUT(Foo, obj, serializer)
{
    return serializer.Put(obj.m_x) &&
        serializer.Put(obj.m_s) &&
        serializer.Put(obj.m_lf);
}

SERIALIZE_DEFINE_GET(Foo, obj, serializer)
{
    return serializer.Get(obj.m_x) &&
        serializer.Get(obj.m_s) &&
        serializer.Get(obj.m_lf);
}


struct FooFoo; // fwd
SERIALIZE_DEFINE_TAG(FooFoo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(FooFoo);

struct FooFoo : public Header
{
    DWORD                   m_child;

    bool Equals(const FooFoo& other) const {
        return ((Header)*this == other) && (m_child == other.m_child);
    }

    SERIALIZE_DECLARE_FRIEND(FooFoo);
};

SERIALIZE_DEFINE_PUT(FooFoo, obj, serializer)
{
    return serializer.Put((Header)obj) &&
        serializer.Put(obj.m_child);
}

SERIALIZE_DEFINE_GET(FooFoo, obj, serializer)
{

    return serializer.Get((Header&)obj) &&  // Note: (Header&)
        serializer.Get(obj.m_child);
}

bool operator==(const FooFoo& lhs, const FooFoo& rhs)
{
    return lhs.Equals(rhs);
}

bool operator!=(const FooFoo& lhs, const FooFoo& rhs)
{
    return !lhs.Equals(rhs);
}



void TestSerializer()
{
    { // builtin types
#       define TEST(typ, val, fmt) \
        { \
        printf("Test_" #typ ": val=" fmt "\n", (typ) val); \
        typ x = (typ) val; \
        TestSerialize(x); \
        } \

        TEST( char, 24, "%d" );
        TEST( char, -42, "%d" );
        TEST( UCHAR, 34, "%u" );
        TEST( wchar_t, 61, "%u" );
        TEST( short, 10043, "%d" );
        TEST( short, -3482, "%d" );
        TEST( USHORT, 32422, "%u" );
        TEST( int, 33, "%d" );
        TEST( int, 100223, "%d" );
        TEST( DWORD, 456456, "%u" );
        TEST( float, 1.0, "%f" );
        TEST( float, 12345.678, "%f" );
        TEST( double, 12345.678, "%f" );
        TEST( bool, true, "%d" );
        TEST( bool, false, "%d" );
    } // builtin types

    { // string
        TestSerializeString(std::string("testing"));
        TestSerializeString(std::string("testing again"));

        TestSerializeString(std::wstring(L"testing"));
        TestSerializeString(std::wstring(L"testing again"));

        TestSerializeCharPtr("testing char*");
        TestSerializeCharPtr("testing char* again");


        TestSerializeCharPtr(L"testing wchar*");
        TestSerializeCharPtr(L"testing wchar* again");
    } // string

    { // array
        // int array
        DWORD a1[4] = { 1, 2, 3, 4 };
        TestSerializeArray(a1, 4);

        // float array
        float a2[5] = { 0.5f, -1.5f, 33.44f, 120.3f, 100.1f };
        TestSerializeArray(a2, 5);

        // string array
        std::string a4[3];
        a4[0] = "zero";
        a4[1] = "one";
        a4[2] = "two";
        TestSerializeArray(a4, 3);

        std::wstring a5[3];
        a5[0] = L"zero";
        a5[1] = L"one";
        a5[2] = L"two";
        TestSerializeArray(a5, 3);

        wchar_t a6[] = { L"wchar" };
        TestSerializeArray(a6, 5);
    } // array

    { // raw(char) data
        UCHAR a1[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        TestSerializeRaw(a1, 8);        
    } // raw data


    { // STL pair
        printf("pair:\n");
        std::pair<DWORD, std::string> obj;
        obj.first = 100;
        obj.second = "foobar";
        TestSerialize(obj);
    } // pair

    { // list
        printf("list:\n");
        std::list<short> obj;
        obj.push_back(1);
        obj.push_back(34);
        obj.push_back(42);
        TestSerialize(obj);
    } // list

    { // vector
        printf("vector:\n");
        std::vector<float> obj;
        obj.push_back(1.0f);
        obj.push_back(3.14159f);
        obj.push_back(66.88f);
        TestSerialize(obj);
    } // vector

    { // deque
        printf("deque:\n");
        std::deque<std::string> obj;
        obj.push_back("one");
        obj.push_back("two");
        TestSerialize(obj);
    } // deque

    // set
    {
        printf("set:\n");
        std::set<int> obj;
        obj.insert(10023);
        obj.insert(34823);
        obj.insert(777777);
        TestSerialize(obj);
    }

    { // multiset
        printf("multiset:\n");
        std::multiset<int> obj;
        obj.insert(10023);
        obj.insert(34824);
        obj.insert(10023);
        TestSerialize(obj);
    } // multiset

    { // map
        printf("map:\n");
        std::map<char,double> obj;
        obj[1] = 10034.32;
        obj[6] = 384234.28;
        TestSerialize(obj);
    } // map

    { // multimap
        printf("multimap:\n");
        std::multimap<UCHAR,float> obj;
        typedef std::multimap<UCHAR,float>::value_type valtype;
        obj.insert( valtype(1, 10034.24f) );
        obj.insert( valtype(2, 348.4234f) );
        obj.insert( valtype(1, 98234.3471f) );
        obj.insert( valtype(2, 10034.24f) );
        TestSerialize(obj);
    } // multimap

    { // user-defined class
        printf("user-defined class:\n");
        Foo f;
        std::vector<Header> l;
        Header  head;        
        l.push_back(head);
        f.Init(1, "hi", l);
        TestSerialize(f);

        // Foo vector
        //*
        std::vector<Foo> vf;
        vf.push_back(f);
        TestSerialize(vf);
        //l.push_back(890.123f);
        f.Init(2, "there", l);
        vf.push_back(f);
        TestSerialize(vf);
        l.clear();
        f.Init(3, "asdf", l);
        vf.push_back(f);
        TestSerialize(vf);
        /**/
    } // user-defined class

    { // KSerBuffer serialize test
        printf("buffer serialize test:\n");
        KSerBuffer buf;
        KSerializer s;
        BOOST_CHECK( s.BeginWriting(&buf, g_bUseTagging) );
        BOOST_CHECK( s.Put("hello, world") );
        BOOST_CHECK( s.Put((DWORD) 42) );
        BOOST_CHECK( s.EndWriting() );

        // serialize that buffer
        TestSerialize(buf);
    } // buffer serialize test

    { // Keyed Serializer test
        printf("Keyed Serializer test:\n");
        { // test 1
            // put some items in
            KKeyedSerializer<USHORT> ks;
            ks.SetTagging(g_bUseTagging);
            BOOST_CHECK( ks.Put(3, std::string("testing")) );
            BOOST_CHECK( ks.Put(4, (DWORD) 342) );
            std::list<float> l;
            l.push_back(1.0);
            l.push_back(2.0);
            BOOST_CHECK( ks.Put(5, l) );

            // check that it serializes
            TestSerialize(ks);

            // now get stuff out
            DWORD ui32;
            BOOST_CHECK( ks.Get(4, ui32) );
            BOOST_CHECK( ui32 == 342 );
            std::list<float> lf;
            BOOST_CHECK( ks.Get(5, lf) );
            BOOST_CHECK( l == lf );
            std::string str;
            BOOST_CHECK( ks.Get(3, str) );
            BOOST_CHECK( str == "testing" );

            // get something again
            ui32 = 1;
            BOOST_CHECK( ks.Get(4, ui32) );
            BOOST_CHECK( ui32 == 342 );
        }

        { // test 2
            // put some items in
            KKeyedSerializer<std::string> ks;
            ks.SetTagging(g_bUseTagging);
            BOOST_CHECK( ks.Put("foo", std::string("bar")) );
            BOOST_CHECK( ks.Put("baz", std::string("quux")) );
            BOOST_CHECK( ks.Put("x", (DWORD) 1) );
            std::set<float> s;
            s.insert(1.0f);
            s.insert(3.4f);
            BOOST_CHECK( ks.Put("zz", s) );

            // check that it serializes
            TestSerialize(ks);

            // now get stuff out
            DWORD ui32;
            BOOST_CHECK( ks.Get("x", ui32) );
            BOOST_CHECK( ui32 == 1 );
            std::string str;
            BOOST_CHECK( ks.Get("baz", str) );
            BOOST_CHECK( str == "quux" );
            BOOST_CHECK( ks.Get("foo", str) );
            BOOST_CHECK( str == "bar" );
            std::set<float> sf;
            BOOST_CHECK( ks.Get("zz", sf) );
            BOOST_CHECK( sf == s );

            // get something again
            str = "x";
            BOOST_CHECK( ks.Get("foo", str) );
            BOOST_CHECK( str == "bar" );
        }
    } // Keyed Serializer test

    { // Net Serializer test
        printf("NetSerializer test:\n");
        TestNetSerialize();
    } // Net Serializer test


    { // user-defined class(hier.)
        printf("user-defined class(hier.):\n");
        FooFoo f;
        f.m_parent = 10;
        f.m_child = 255;
        TestSerialize(f);
    } // user-defined class 
  
    { // Compress
        std::string strTest( "나랏말사미 듕귁에 다라 문자와 서로 사맛디 아니할세 이런전챠로 새로이 .... " );
  
        KSerializer     s;
        KSerBuffer      buf;
        KSerBuffer      buf2;
 
        // write
        BOOST_CHECK( s.BeginWriting(&buf) );
        BOOST_CHECK( s.Put(strTest) );
        BOOST_CHECK( s.EndWriting() );

        buf2 = buf; // buf2에 comp / uncomp 한 후, buf에 있는 원본과 비교한다.
        BOOST_CHECK( buf == buf2 );

        DumpBuffer(buf);
        std::cout << "Before Compressed Size : " << buf.GetLength() << std::endl;
        BOOST_CHECK( buf.Compress() );
        std::cout << "After Compressed Size : " << buf.GetLength() << std::endl;
        BOOST_CHECK( buf.UnCompress() );
        std::cout << "After UnCompressed Size : " << buf.GetLength() << std::endl;
        BOOST_CHECK( buf == buf2 );

    } // Compress


} //main()
