#pragma once

#include "FSM/FSMclass.h"
#include "FSM/FSMstate.h"
#include <Thread/Locker.h>


#define DeclareFSM  \
public: \
    void SetFSM( FSMclassPtr& spFSM ) \
    { \
        m_spFSM = spFSM; \
        m_pkCurrentState = m_spFSM->GetState(1); \
    } \
    FSMclassPtr GetFSM() \
    { \
        return m_spFSM; \
    } \
    const wchar_t* GetStateIDString() const \
    { \
        return m_spFSM->GetStateIDString(m_pkCurrentState->GetID()); \
    } \
    int GetStateID() \
    { \
        return m_pkCurrentState->GetID(); \
    } \
    void StateTransition( int nInput ) \
    { \
        m_pkCurrentState = m_spFSM->GetState( m_pkCurrentState->GetOutput( nInput ) ); \
    } \
    void ForceStateTransitionTo( int nStateID ) \
    { \
        m_pkCurrentState = m_spFSM->GetState( nStateID ); \
    } \
protected: \
    FSMstate*                       m_pkCurrentState;   \
    FSMclassPtr                     m_spFSM



#define DeclareFSM_MT  \
public: \
    void SetFSM( FSMclassPtr& spFSM ) \
    { \
        KLocker lock( m_cs ); \
        m_spFSM = spFSM; \
        m_pkCurrentState = m_spFSM->GetState(1); \
    } \
    FSMclassPtr GetFSM() \
    { \
        KLocker lock( m_cs ); \
        return m_spFSM; \
    } \
    const wchar_t* GetStateIDString() const \
    { \
        KLocker lock( m_cs ); \
        return m_spFSM->GetStateIDString(m_pkCurrentState->GetID()); \
    } \
    int GetStateID() \
    { \
        KLocker lock( m_cs ); \
        return m_pkCurrentState->GetID(); \
    } \
    void StateTransition( int nInput ) \
    { \
        KLocker lock( m_cs ); \
        m_pkCurrentState = m_spFSM->GetState( m_pkCurrentState->GetOutput( nInput ) ); \
    } \
    void ForceStateTransitionTo( int nStateID ) \
    { \
        KLocker lock( m_cs ); \
        m_pkCurrentState = m_spFSM->GetState( nStateID ); \
    } \
protected: \
    mutable KncCriticalSection      m_cs;   \
    FSMstate*                       m_pkCurrentState;   \
    FSMclassPtr                     m_spFSM




#define VERIFY_FORBIDDEN_STATE( varg ) \
	if( m_pkCurrentState->CheckState varg ) \
	{ \
	START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
	<< L"        현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L" 허용 안되는 상태 : "L ## #varg << END_LOG; \
	return; \
	}

#define VERIFY_STATE_LOG( log, varg ) \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
	START_LOG( log, L"상태 오류. Name : " << m_strName ) \
	<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
	<< L"    허용 상태 : "L ## #varg << END_LOG; \
	return; \
	}

#define VERIFY_STATE( varg ) \
    if( !m_pkCurrentState->CheckState varg ) \
    { \
        START_LOG( cerr, L"상태 오류. Name : " << m_strName ) \
            << L"    현재 상태 : " << GetStateIDString() << dbg::endl \
			<< L"    허용 상태 : "L ## #varg << END_LOG; \
        return; \
    }

#define VERIFY_STATE_WARN( varg ) \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
		START_LOG( cwarn, L"상태 오류. Name : " << m_strName ) \
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
			<< L"    허용 상태 : "L ## #varg << END_LOG; \
		return; \
	}

#define _VERIFY_STATE_ACK( varg, eventid, packet ) \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
		START_LOG( cerr, L"상태 오류. [return ack] Name : " << m_strName ) \
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
			<< L"    허용 상태 : "L ## #varg << END_LOG; \
		packet kAck; \
		kAck.m_iOK = NetError::ERR_WRONG_STATE_00; \
		SendPacket( eventid, kAck ); \
		return; \
	}

#define VERIFY_STATE_ACK( varg, eventid )		_VERIFY_STATE_ACK( varg, eventid, K##eventid )

#define _VERIFY_STATE_ACK_WARN( varg, eventid, packet ) \
	if( !m_pkCurrentState->CheckState varg ) \
	{ \
		START_LOG( cwarn, L"상태 오류. [return ack] Name : " << m_strName ) \
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl \
			<< L"    허용 상태 : "L ## #varg << END_LOG; \
		packet kAck; \
		kAck.m_iOK = NetError::ERR_WRONG_STATE_00; \
		SendPacket( eventid, kAck ); \
		return; \
	}

#define VERIFY_STATE_ACK_WARN( varg, eventid )		_VERIFY_STATE_ACK_WARN( varg, eventid, K##eventid )


