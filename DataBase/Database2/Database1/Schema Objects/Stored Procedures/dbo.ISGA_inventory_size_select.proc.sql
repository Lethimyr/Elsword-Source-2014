CREATE PROCEDURE [dbo].[ISGA_inventory_size_select]
	@iLoginUID_ [int],
	@iTotalSize [smallint] = 260
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081008. microcat.
*/

select
    @iTotalSize = Size
from dbo.GInventoryInfo
where
    LoginUID = @iLoginUID_

select
    @iTotalSize


