#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>

class KMemUsageCheckManager
{
public:
	class KMemUsage
	{
	public:
		KMemUsage() { Init( -1 ); }

		void Init( int iGameSpace )
		{
			m_fMinWorkingSetSize = -1.0f;
			m_fMaxWorkingSetSize = -1.0f;
			m_fMinPagefileUsage = -1.0f;
			m_fMaxPagefileUsage = -1.0f;

			m_fLastCheckTime = -1.0f;

			//
			m_iLastGameSpace = iGameSpace;
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		float GetMinWorkingSetSize() const { return m_fMinWorkingSetSize; }
		float GetMaxWorkingSetSize() const { return m_fMaxWorkingSetSize; }
		float GetMinPagefileUsage() const { return m_fMinPagefileUsage; }
		float GetMaxPagefileUsage() const { return m_fMaxPagefileUsage; }
		
		void Update();

	private:
		float m_fMinWorkingSetSize;		//최소 작업 메모리 사용량
		float m_fMaxWorkingSetSize;		//최대 작업 메모리 사용량
		float m_fMinPagefileUsage;			//페이지파일 사용량
		float m_fMaxPagefileUsage;		//페이지파일 사용량
		float m_fLastCheckTime;//마지막 체크 시간

		//
		int m_iLastGameSpace;
	};


	//
	KMemUsageCheckManager();
	~KMemUsageCheckManager();

	//
	void Init( float fUpdateTime );

	//
	void UpdateMemUsage();

	//
	void Start();
	void End();

	const KMemUsage& GetMemUsage() { return m_kMemUsage; }

private:
	//
	float m_fUpdateTime;

	KMemUsage m_kMemUsage;//메모리 사용량

	//checking 여부
	bool m_bChecking;
};