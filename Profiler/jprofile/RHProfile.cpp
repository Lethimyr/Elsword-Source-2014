/***************************************************************************************************
**
** RHProfile.cpp
**
** Real-Time Hierarchical Profiling for Game Programming Gems 3
**
** by Greg Hjelstrom & Byon Garrabrant
**
***************************************************************************************************/

/** @file   RHProfile.cpp
    @desc   implementation of RHProfile
*/

#define _TOSTR1(x)    #x
#define _TOSTR(x)     _TOSTR1(x)
#include _TOSTR(PROJECT_PCH)
#include "RHProfile.h"
#include <windows.h>


inline void Profile_Get_Ticks(__int64 * ticks)
{
    __asm
    {
        push    edx;
        push    ecx;
        mov     ecx,ticks;
        _emit   0Fh
        _emit   31h
        mov     [ecx],eax;
        mov     [ecx+4],edx;
        pop     ecx;
        pop     edx;
    }
}//Profile_Get_Ticks()


inline float Profile_Get_Tick_Rate(void)
{
    static float _CPUFrequency = -1.0f;
    
    if (_CPUFrequency == -1.0f)
    {
        __int64 curr_rate = 0;
        ::QueryPerformanceFrequency ((LARGE_INTEGER *)&curr_rate);
        _CPUFrequency = (float)curr_rate;
    } 
    
    return _CPUFrequency;
}//Profile_Get_Tick_Rate()


/***************************************************************************************************
**
** RHProfileNode
**
***************************************************************************************************/

/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - pointer to a static string which is the name of this profile node                    *
 * parent - parent pointer                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The name is assumed to be a static pointer, only the pointer is stored and compared for     *
 * efficiency reasons.                                                                         *
 *=============================================================================================*/
RHProfileNode::RHProfileNode( const char * name, RHProfileNode * parent ) :
    Name( name ),
    TotalCalls( 0 ),
    TotalTime( 0 ),
    StartTime( 0 ),
    RecursionCounter( 0 ),
    Parent( parent ),
    Child( NULL ),
    Sibling( NULL )
{
    Reset();
}//RHProfileNode::RHProfileNode()


RHProfileNode::~RHProfileNode( void )
{
	DeleteChildAndSibling();
}//RHProfileNode::~RHProfileNode()


/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - static string pointer to the name of the node we are searching for                   *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * All profile names are assumed to be static strings so this function uses pointer compares   *
 * to find the named node.                                                                     *
 *=============================================================================================*/
RHProfileNode * RHProfileNode::Get_Sub_Node( const char * name )
{
    // Try to find this sub node
    RHProfileNode * child = Child;
    while ( child )
    {
        if ( child->Name == name )
        {
            return child;
        }
        child = child->Sibling;
    }

    // We didn't find it, so add it
    RHProfileNode * node = new RHProfileNode( name, this );
    node->Sibling = Child;
    Child = node;
    return node;
}//RHProfileNode::Get_Sub_Node()


void RHProfileNode::Reset( void )
{
    TotalCalls = 0;
    TotalTime = 0.0f;

    if ( Child ) {
        Child->Reset();
    }
    if ( Sibling ) {
        Sibling->Reset();
    }
}//RHProfileNode::Reset()


void RHProfileNode::Call( void )
{
    TotalCalls++;
    if (RecursionCounter++ == 0)
    {
        Profile_Get_Ticks(&StartTime);
    }
}//RHProfileNode::Call()


bool RHProfileNode::Return( void )
{
    if ( --RecursionCounter == 0 && TotalCalls != 0 )
    {
        __int64 time;
        Profile_Get_Ticks(&time);
        time-=StartTime;
        TotalTime += (float)time / Profile_Get_Tick_Rate();
    }
    return ( RecursionCounter == 0 );
}//RHProfileNode::Return()

void RHProfileNode::DeleteChildAndSibling()
{
	if( Child )
	{
		delete Child;
		Child = NULL;
	}

	if( Sibling )
	{
		delete Sibling;
		Sibling = NULL;
	}
}


/***************************************************************************************************
**
** RHProfileIterator
**
***************************************************************************************************/
RHProfileIterator::RHProfileIterator( RHProfileNode * start )
{
    CurrentParent = start;
    CurrentChild = CurrentParent->Get_Child();
}//RHProfileIterator::RHProfileIterator()


void RHProfileIterator::First(void)
{
    CurrentChild = CurrentParent->Get_Child();
}//RHProfileIterator::First()


void RHProfileIterator::Next(void)
{
    CurrentChild = CurrentChild->Get_Sibling();
}//RHProfileIterator::Next()


bool RHProfileIterator::Is_Done(void)
{
    return CurrentChild == NULL;
}//RHProfileIterator::Is_Done()


void RHProfileIterator::Enter_Child( int index )
{
    CurrentChild = CurrentParent->Get_Child();
    while ( (CurrentChild != NULL) && (index != 0) )
    {
        index--;
        CurrentChild = CurrentChild->Get_Sibling();
    }

    if ( CurrentChild != NULL )
    {
        CurrentParent = CurrentChild;
        CurrentChild = CurrentParent->Get_Child();
    }
}//RHProfileIterator::Enter_Child()


void RHProfileIterator::Enter_Parent( void )
{
    if ( CurrentParent->Get_Parent() != NULL )
    {
        CurrentParent = CurrentParent->Get_Parent();
    }
    CurrentChild = CurrentParent->Get_Child();
}//RHProfileIterator::Enter_Parent()


/***************************************************************************************************
**
** RHProfileManager
**
***************************************************************************************************/

RHProfileNode   RHProfileManager::Root( "Root", NULL );
RHProfileNode * RHProfileManager::CurrentNode = &RHProfileManager::Root;
int             RHProfileManager::FrameCounter = 0;
__int64         RHProfileManager::ResetTime = 0;
BOOL            RHProfileManager::ms_bDoProfile = TRUE;


/***********************************************************************************************
 * RHProfileManager::Start_Profile -- Begin a named profile                                    *
 *                                                                                             *
 * Steps one level deeper into the tree, if a child already exists with the specified name     *
 * then it accumulates the profiling; otherwise a new child node is added to the profile tree. *
 *                                                                                             *
 * INPUT:                                                                                      *
 * name - name of this profiling record                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The string used is assumed to be a static string; pointer compares are used throughout      *
 * the profiling code for efficiency.                                                          *
 *=============================================================================================*/
void RHProfileManager::Start_Profile( const char * name )
{
    if (name != CurrentNode->Get_Name())
    {
        CurrentNode = CurrentNode->Get_Sub_Node( name );
    }
    
    CurrentNode->Call();
}//RHProfileManager::Start_Profile()


/***********************************************************************************************
 * RHProfileManager::Stop_Profile -- Stop timing and record the results.                       *
 *=============================================================================================*/
void RHProfileManager::Stop_Profile( void )
{
    // Return will indicate whether we should back up to our parent (we may
    // be profiling a recursive function)
    if (CurrentNode->Return())
    {
        CurrentNode = CurrentNode->Get_Parent();
    }
}//RHProfileManager::Stop_Profile()


/***********************************************************************************************
 * RHProfileManager::Reset -- Reset the contents of the profiling system                       *
 *                                                                                             *
 *    This resets everything except for the tree structure.  All of the timing data is reset.  *
 *=============================================================================================*/
void RHProfileManager::Reset( void )
{ 
    Root.Reset(); 
    FrameCounter = 0;
    Profile_Get_Ticks(&ResetTime);
}//RHProfileManager::Reset()


/*static*/ void RHProfileManager::ResetRoot()
{
    Root.Reset();
}//RHProfileManager::ResetRoot()

/*static*/ void RHProfileManager::ClearRoot()
{
	CurrentNode = &Root;
	Root.DeleteChildAndSibling();
	Root.Reset();
}


/***********************************************************************************************
 * RHProfileManager::Increment_Frame_Counter -- Increment the frame counter                    *
 *=============================================================================================*/
void RHProfileManager::Increment_Frame_Counter( void )
{
    FrameCounter++;
}//RHProfileManager::Increment_Frame_Counter()


/***********************************************************************************************
 * RHProfileManager::Get_Time_Since_Reset -- returns the elapsed time since last reset         *
 *=============================================================================================*/
float RHProfileManager::Get_Time_Since_Reset( void )
{
    __int64 time;
    Profile_Get_Ticks(&time);
    time -= ResetTime;
    return (float)time / Profile_Get_Tick_Rate();
}//RHProfileManager::Get_Time_Since_Reset()
