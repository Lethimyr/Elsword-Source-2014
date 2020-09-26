//{{ kimhc // 2009-11-02 // 유저 자신에 의해 길드 경험치가 올랐을 때 실행 되는 informer
#ifdef	GUILD_MANAGEMENT
#pragma once

class CX2InformerGuildExp : public CX2Informer
{
public:

	CX2InformerGuildExp(void);
	virtual ~CX2InformerGuildExp(void);

	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
	void Reset();
	inline void SetIncreaseGuildExpByMe( bool bChange ) { m_bIncreaseGuildExpByMe = bChange; }
	inline bool GetIncreaseGuildExpByMe() const { return m_bIncreaseGuildExpByMe; }

protected:

	void SetNowInfo();
	bool IsNowInfoChange();
	void SetShow( bool bShow );

private:
	bool m_bIncreaseGuildExpByMe;				// 본인에 의해 길드 경험치가 올라 갔는가?
};
#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-11-02 // 유저 자신에 의해 길드 경험치가 올랐을 때 실행 되는 informer