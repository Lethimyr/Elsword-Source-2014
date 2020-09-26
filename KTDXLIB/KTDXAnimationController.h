#pragma once


#define KOGANIMATIONCONTROLLER_FORMAT_MAGIC           (MAKEFOURCC('K','A','C',' '))
#define KOGANIMATIONCONTROLLER_FORMAT_VERSION         0x0100

namespace   KTDXAnimationController
{
    // D3DXLoadMeshHierarchyFromX 함수에 의해 생성된 ID3DXAnimationController의 내용을 자체포맷으로 저장하는 함수
    // 일반적인 ID3DXAnimationController 에는 적용될 수 없다.
    HRESULT WriteToFileSerializer( KFileSerializer& kInOutFile_, const CKTDXDeviceXSkinMesh* pInSkinMesh_, LPD3DXANIMATIONCONTROLLER pInAnimCon_ );

    LPD3DXANIMATIONCONTROLLER   CloneOptimizedAC( const CKTDXDeviceXSkinMesh* pInSkinMesh_, LPD3DXANIMATIONCONTROLLER pInOutAnimCon_ );

    HRESULT ReadFromMemory( DWORD dwInSize_, const void* pInData_, CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, LPD3DXANIMATIONCONTROLLER* ppInAC_ );

}//namespace   KTDXAnimationController
