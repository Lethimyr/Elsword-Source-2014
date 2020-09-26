CREATE PROCEDURE [dbo].[CSGA_couple_system_select_warehouse]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081028. microcat.

{ call dbo.CSGA_couple_system_select_warehouse ( %d ) }

{ call dbo.CSGA_couple_system_select_warehouse ( @1 ) }
@1 ; LoginUID int

n return ( @1 )
@1 ; ItemUID int
@1 ; ItemID int
*/

select
    ItemUID
,   ItemID
from dbo.CSGACoupleSystemWarehouse
where
    LoginUID = @iLoginUID_
option (maxdop 1)


