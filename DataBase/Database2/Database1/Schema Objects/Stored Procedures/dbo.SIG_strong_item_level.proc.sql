CREATE PROCEDURE [dbo].[SIG_strong_item_level]
	@iItemID_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

select
    Kind
,   Level
,   CharType
from dbo.SIGStrongItem
where
    ItemID = @iItemID_


