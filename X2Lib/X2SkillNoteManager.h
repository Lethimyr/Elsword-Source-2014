#pragma once

/*
	#현재 선택된 캐릭터 클래스의 메모 리스트를 관리 하는 클래스
*/
#ifdef REFORM_SKILL_NOTE_UI
class CX2SkillNoteManager
{
public:
	struct MemoData
	{
		int iMemoItemID;
		int iSkillID;
		int iMemoItemNum;	// 메모 아이템 소지량
		char cClassLine;		// 정렬을 위한 클래스 라인 구분
			// 노전~2차 (0~2)
	};

	// 메모 리스트 정렬 클래스
	struct MemoListSort : public std::binary_function< MemoData, MemoData, bool >
	{
		bool operator()( const MemoData& _Left, const MemoData& _Right ) const
		{
			if( (_Left.iMemoItemNum > 0) == (_Right.iMemoItemNum > 0) )
			{
				// 전직 낮은 순 우선
				if( _Left.cClassLine < _Right.cClassLine )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if ( (_Left.iMemoItemNum > 0) > (_Right.iMemoItemNum > 0) )
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};

	struct MemoListClassSort : public std::binary_function< MemoData, MemoData, bool >
	{
		bool operator()( const MemoData& _Left, const MemoData& _Right ) const
		{
			// 전직 낮은 순 우선
			if( _Left.cClassLine < _Right.cClassLine )
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};
public:
	static CX2SkillNoteManager* GetInstance();
	static void ResetSkillNoteManager();
	static void DestroyInstance();
	///////////////////////////////////////////////////////

	// 스크립트 파싱 관련
	void AddSkillNoteTemplet_LUA();
	bool OpenScriptFile();

	// 클래스 설정
	void SetUnitClass( CX2Unit::UNIT_CLASS eUnitClass_ ) { m_eUnitClass = eUnitClass_; }
	CX2Unit::UNIT_CLASS GetUnitClass() const { return m_eUnitClass; } 

	// 메모 리스트 갱신
	void ResetMemoList(); 
	void UpdateMemoPossession();
	void UpdateSortMemoList();

	// 메모 리스트 관리
	const vector<MemoData>& GetUsableMemoListIfIHave(){ return m_vecUsableMemoListIfIhave; }
	int GetSkillIDByMemoItemID( int iMemoItemID_, bool bIsMySkill_ = true ) const;

	// ~End Public Function
private:

	// ~End Private Function
private:
	CX2SkillNoteManager();
	~CX2SkillNoteManager();
	
	static CX2SkillNoteManager*							m_pManager;
	//////////////////////////////////////////////////////

	// 현재 선택된 캐릭터의 유닛 클래스
	CX2Unit::UNIT_CLASS									m_eUnitClass; 

	// key : UnitClass,  val : ( 메모아이템ID, 스킬ID)	
	map< CX2Unit::UNIT_CLASS, vector<pair<int, int>> >	m_mapSkillNoteTemplet;	

	// (보유 했을 때)현재 선택된 캐릭터의 클래스가 사용 가능한 메모 리스트	
	vector<MemoData>									m_vecUsableMemoListIfIhave;	
};

#endif // REFORM_SKILL_NOTE_UI

