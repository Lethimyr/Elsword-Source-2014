CREATE PROCEDURE [dbo].[CSGA_couple_system_select_equip]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_select_equip ( %d ) }

{ call dbo.CSGA_couple_system_select_equip ( @1 ) }
@1 ; LoginUID int

n return ( @1, @2, @3, @4 )
@1 ; CharType smallint ; -1(유저), 0부터 엘리시스...
@2 ; Type tinyint ; 커플 아이템 장착 위치
@3 ; ItemUID int
@4 ; ItemID int
*/

select
    CharType
,   Type
,   ItemUID
,   ItemID
from dbo.CSGACoupleSystemEquip
where
    LoginUID = @iLoginUID_
option (maxdop 1)


