#pragma once
#include "ktdxdevice.h"


class CKTDXDeviceBaseTexture : public CKTDXDevice
{
public:
	CKTDXDeviceBaseTexture( wstring deviceID, DEVICETYPE eDeviceType );
	virtual void SetDeviceTexture( int stage = 0 ) = 0;

protected:
	virtual ~CKTDXDeviceBaseTexture(void);

};

