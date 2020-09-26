CREATE PROCEDURE [dbo].[gii_indigo_island_rank]
	@iRank_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081114. microcat.

{ call dbo.gii_indigo_island_rank ( %d ) }

{ call dbo.gii_indigo_island_rank ( @1 ) }
@1 ; Rank int ; 255를 초과할 경우 255로 고정됨

n return ( @1, @2, @3, @4, @5, @6, @7, @8 )
@1 ; Rank tinyint
@2 ; Nick nvarchar(24)
@3 ; CharType tinyint
@4 ; RP int
@5 ; Win int
@6 ; Lose int
@7 ; WinRate float
@8 ; Change int
*/

select
    @iRank_ =
    case
        when @iRank_ > 255 then 255
        else @iRank_
    end

select
    Rank
,   Nick
,   CharType
,   RP
,   Win
,   Lose
,   WinRate
,   Change
from dbo.GIIIndigoIslandRank
where
    Rank between 1 and @iRank_
option (maxdop 1)


