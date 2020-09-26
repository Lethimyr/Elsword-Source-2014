#pragma once

class CX2StateStartUp : public CX2State
{
public:
	CX2StateStartUp(void);
	virtual ~CX2StateStartUp(void);

protected:
	void LoadData();

#ifdef FIRST_SCREEN_TEST
	public:
		virtual HRESULT OnFrameRender();

	private:
		CKTDXDeviceTexture*		m_pTexutureBG;
#endif // FIRST_SCREEN_TEST

};
