/***************************************************************************************************
**
** RHProfile.h
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/
#pragma once

#define _PROFILE

#ifdef _PROFILE
#define KTDXPROFILE()               RHProfileSample __profile( __FUNCTION__ )
#define KTDXPROFILE2(name)          RHProfileSample __profile( name )
#define KTDXPROFILE_BEGIN(name)     { RHProfileSample __profile( name )
#define KTDXPROFILE_END()           }
#else
#define KTDXPROFILE()
#define KTDXPROFILE2(name)
#define KTDXPROFILE_BEGIN(name)
#define KTDXPROFILE_END()
#endif // _PROFILE


#ifndef _RHPROFILE_H
#define _RHPROFILE_H

typedef int BOOL;

/*
** A node in the Profile Hierarchy Tree
*/
class RHProfileNode
{
public:
    RHProfileNode( const char * name, RHProfileNode * parent );
    ~RHProfileNode( void );

    RHProfileNode * Get_Sub_Node( const char * name );

    RHProfileNode * Get_Parent( void )      { return Parent; }
    RHProfileNode * Get_Sibling( void )     { return Sibling; }
    RHProfileNode * Get_Child( void )       { return Child; }

    void            Reset( void );
    void            Call( void );
    bool            Return( void );

    const char *    Get_Name( void )            { return Name; }
    int             Get_Total_Calls( void )     { return TotalCalls; }
    float           Get_Total_Time( void )      { return TotalTime; }

	void DeleteChildAndSibling();

protected:
    const char *    Name;
    int             TotalCalls;
    float           TotalTime;
    __int64         StartTime;
    int             RecursionCounter;

    RHProfileNode * Parent;
    RHProfileNode * Child;
    RHProfileNode * Sibling;
};//class RHProfileNode


/*
** An iterator to navigate through the tree
*/
class RHProfileIterator
{
public:
    // Access all the children of the current parent
    void    First(void);
    void    Next(void);
    bool    Is_Done(void);

    void    Enter_Child( int index );       // Make the given child the new parent
    void    Enter_Largest_Child( void );    // Make the largest child the new parent
    void    Enter_Parent( void );           // Make the current parent's parent the new parent

    // Access the current child
    const char *    Get_Current_Name( void )            { return CurrentChild->Get_Name(); }
    int             Get_Current_Total_Calls( void ) { return CurrentChild->Get_Total_Calls(); }
    float           Get_Current_Total_Time( void )  { return CurrentChild->Get_Total_Time(); }

    // Access the current parent
    const char *    Get_Current_Parent_Name( void )         { return CurrentParent->Get_Name(); }
    int             Get_Current_Parent_Total_Calls( void )  { return CurrentParent->Get_Total_Calls(); }
    float           Get_Current_Parent_Total_Time( void )   { return CurrentParent->Get_Total_Time(); }

protected:
    RHProfileNode * CurrentParent;
    RHProfileNode * CurrentChild;

    RHProfileIterator( RHProfileNode * start );
    friend class RHProfileManager;
};//class RHProfileIterator


/*
** The Manager for the Profile system
*/
class RHProfileManager
{
public:
    static  void    Start_Profile( const char * name );
    static  void    Stop_Profile( void );

    static  void    Reset( void );
    static  void    ResetRoot();
	static  void	ClearRoot();
    static  void    Increment_Frame_Counter( void );
    static  int     Get_Frame_Count_Since_Reset( void )     { return FrameCounter; }
    static  float   Get_Time_Since_Reset( void );

    static  RHProfileIterator * Get_Iterator( void )    { return new RHProfileIterator( &Root ); }
    static  void                Release_Iterator( RHProfileIterator * iterator ) { delete iterator; }

private:
    static  RHProfileNode       Root;
    static  RHProfileNode *     CurrentNode;
    static  int                 FrameCounter;
    static  __int64             ResetTime;

public:
    static  BOOL                ms_bDoProfile;
};//class RHProfileManager


/*
** ProfileSampleClass is a simple way to profile a function's scope
** Use the PROFILE macro at the start of scope to time
*/
class RHProfileSample
{
public:
    RHProfileSample( const char * name )
    { 
        if ( RHProfileManager::ms_bDoProfile )
            RHProfileManager::Start_Profile( name ); 
    }
    
    ~RHProfileSample( void )
    { 
        if ( RHProfileManager::ms_bDoProfile )
            RHProfileManager::Stop_Profile(); 
    }
};//class RHProfileSample

#endif // _RHPROFILE_H


/** @example    RHProfile

    #include "KJohnGen.h"
    #include <iostream>
    #include <conio.h>
    #include "RHProfile.h"
    #include "KFileEx.h"

    using namespace std;

    class TGameApp
    {
    public:
        void OnFrameMove();
        void OnRender();
    };//class TGameApp

    void TGameApp::OnFrameMove()
    {
        PROFILE( "OnFrameMove" );
        Sleep(100);
    }//TGameApp::OnFrameMove()


    void TGameApp::OnRender()
    {
        PROFILE( "OnRender" );
        Sleep(200);
    }//TGameApp::OnRender()


    TGameApp            g_app;
    RHProfileIterator*  g_pitor = NULL;

    void main()
    {
        g_pitor = RHProfileManager::Get_Iterator();

        while ( !kbhit() )
        {
            //Sleep(500);
            static int ii = 0;
            printf( "%i\n", ++ii );
            RHProfileManager::Increment_Frame_Counter();
            g_app.OnFrameMove();
            g_app.OnRender();
        }//while

        // write profiling info to file
        KFileEx file;

        file.Open( "profile.txt", KFileEx::READWRITE );
        if ( file.GetHandle() == -1 )
            return;

        char    buf[1024];

        g_pitor->First();
        while ( FALSE == g_pitor->Is_Done() )
        {
            sprintf( buf, "%s, calls=%i, time=%g(parent=%s, calls=%i, time=%g)\n",
                g_pitor->Get_Current_Name(),
                g_pitor->Get_Current_Total_Calls(),
                g_pitor->Get_Current_Total_Time(),
                g_pitor->Get_Current_Parent_Name(),
                g_pitor->Get_Current_Parent_Total_Calls(),
                g_pitor->Get_Current_Parent_Total_Time() );
            file.Write( buf, (int)strlen(buf) );
            g_pitor->Next();
        }//while
        file.Close();

        RHProfileManager::Release_Iterator( g_pitor );
    }//main()
    // output of 'profile.txt'
    //
    //OnRender, calls=4, time=0.800769(parent=Root, calls=0, time=0)
    //OnFrameMove, calls=4, time=0.401246(parent=Root, calls=0, time=0)

*/
