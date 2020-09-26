#include "SectorID.h"

namespace KSectorID
{
	int	ExtractX( SectorID iSectorID )
	{
		return (iSectorID >> 16) & static_cast<SectorID>(0x000000ff);
	}

	int	ExtractY( SectorID iSectorID )
	{
		return (iSectorID >> 8) & static_cast<SectorID>(0x000000ff);
	}

	int	ExtractZ( SectorID iSectorID )
	{
		return iSectorID & static_cast<SectorID>(0x000000ff);
	}

    int ExtractAdjacentSector( SectorID iNowSector, int iMaxX, int iMaxY, int iMaxZ, std::vector<SectorID>& vecAdjacentSector )
	{
		vecAdjacentSector.clear();

		if( iNowSector == INVALID_SECTOR )
		{
			return 0;
		}

		int ix = ExtractX( iNowSector );
		int iy = ExtractY( iNowSector );
		int iz = ExtractZ( iNowSector );

        for( int x = ix - 1; x <= ix + 1; x++ )
        {
            if( x < 0 )
            {
                continue;
            }
            else if( x >= iMaxX )
            {
                break;
            }
            for( int y = iy - 1; y <= iy + 1; y++ )
            {
                if( y < 0 )
                {
                    continue;
                }
                else if( y >= iMaxY )
                {
                    break;
                }
                for( int z = iz - 1; z <= iz + 1; z++ )
                {
                    if( z < 0 )
                    {
                        continue;
                    }
                    else if( z >= iMaxZ )
                    {
                        break;
                    }

                    SectorID temp;
                    SetIndex( temp, x, y, z );
                    vecAdjacentSector.push_back( temp );
                }
            }
        }

		return static_cast< int >( vecAdjacentSector.size() );
	}

    void FindRemainder( const std::vector< SectorID >& vecSrcSectorGroup,
                        const std::vector< SectorID >& vecDestSectorGroup,
                        std::vector< SectorID >& vecSrcRemainder,
                        std::vector< SectorID >& vecDestRemainder )
    {
        vecSrcRemainder.clear();
        vecDestRemainder.clear();

        std::vector< SectorID >::const_iterator vitSrc = vecSrcSectorGroup.begin();
        std::vector< SectorID >::const_iterator vitDest = vecDestSectorGroup.begin();

        while( ( vitSrc != vecSrcSectorGroup.end() ) || ( vitDest != vecDestSectorGroup.end() ) )
        {
            if( vitSrc == vecSrcSectorGroup.end() )
            {
                vecDestRemainder.insert( vecDestRemainder.end(), vitDest, vecDestSectorGroup.end() );
                break;
            }

            if( vitDest == vecDestSectorGroup.end() )
            {
                vecSrcRemainder.insert( vecSrcRemainder.end(), vitSrc, vecSrcSectorGroup.end() );
                break;
			}

            if( *vitSrc < *vitDest )
            {
                vecSrcRemainder.push_back( *vitSrc );
                vitSrc++;
                continue;
            }
            else if( *vitSrc > *vitDest )
            {
                vecDestRemainder.push_back( *vitDest );
                vitDest++;
                continue;
            }
            else
            {
                vitSrc++;
                vitDest++;
                continue;
            }
        }
    }

	void SetX( SectorID& iDestID, int iX )
	{
		if( iX > MAX_SECTOR )
			iX = MAX_SECTOR;
		else if( iX < 0 )
			iX = 0;

		iDestID = (iDestID & 0x0000ffff) | (((iX & static_cast<int>(0x000000ff)) << 16) & 0x00ff0000);
	}

	void SetY( SectorID& iDestID, int iY )
	{
		if( iY  > MAX_SECTOR )
			iY  = MAX_SECTOR;
		else if( iY  < 0 )
			iY  = 0;

		iDestID = (iDestID & 0x00ff00ff) | (((iY  & static_cast<int>(0x000000ff)) << 8) & 0x0000ff00);
	}

	void SetZ( SectorID& iDestID, int iZ )
	{
		if( iZ > MAX_SECTOR )
			iZ = MAX_SECTOR;
		else if( iZ < 0 )
			iZ = 0;

		iDestID = (iDestID & 0x00ffff00) | (((iZ & static_cast<int>(0x000000ff))) & 0x000000ff);
	}

	void SetIndex( SectorID& iDestID, int iX, int iY, int iZ )
	{
        iDestID = 0x00000000;

		SetX( iDestID, iX );
		SetY( iDestID, iY );
		SetZ( iDestID, iZ );
	}
}