#pragma once

#include <vector>
#include <KncSmartPtr.h>

#define MAX_SECTOR	255
#define INVALID_SECTOR 0xffffffff

typedef	__int32	SectorID;
typedef boost::shared_ptr< std::vector< SectorID > > SectorsPtr;

/*    32 bit ( 4 byte )

| ff ff ff ff | 

| 00 ff 00 00 | X pos index
| 00 00 ff 00 | Y pos index
| 00 00 00 ff | Z pos index

*/

namespace KSectorID
{
	int		ExtractX( SectorID iSectorID );
	int		ExtractY( SectorID iSectorID );
	int		ExtractZ( SectorID iSectorID );

    // iNowSector를 포함하여 iNowSector에 인접한 섹터들이 오름차순 정렬되어 출력된다. vecAdjacentSector가 출력물이다.
	int		ExtractAdjacentSector( SectorID iNowSector, int iMaxX, int iMaxY, int iMaxZ, std::vector< SectorID >& vecAdjacentSector );

    // vecSrcSectorGroup과 vecDestSectorGroup은 오름차순으로 정렬되어 있어야 한다.
    // 참고로, ExtractAdjacentSector의 출력인 vecAdjacentSector는 오름차순 정렬된다.
    void    FindRemainder( const std::vector< SectorID >& vecSrcSectorGroup,
                           const std::vector< SectorID >& vecDestSectorGroup,
                           std::vector< SectorID >& vecSrcRemainder,
                           std::vector< SectorID >& vecDestRemainder );

	void	SetX( SectorID& iDestID, int iX );
	void	SetY( SectorID& iDestID, int iY );
	void	SetZ( SectorID& iDestID, int iZ );
	void	SetIndex( SectorID& iDestID, int iX, int iY, int iZ );
}
