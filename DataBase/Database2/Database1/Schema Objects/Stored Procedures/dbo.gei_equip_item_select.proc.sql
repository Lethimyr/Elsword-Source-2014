CREATE PROCEDURE [dbo].[gei_equip_item_select]
	@iLoginUID_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081114. microcat.
*/

select
    CharType
,   ItemUID
,   ItemID
,   ItemType
from dbo.GEIEquipItem
where
    LoginUID = @iLoginUID_
option (maxdop 1)


