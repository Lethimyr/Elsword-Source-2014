CREATE PROCEDURE [dbo].[GNGA_game_notice_select_No]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081010. microcat.
*/

select
    No
from dbo.GNGAGameNotice
where
    current_timestamp between StartDateB and EndDateB
option (maxdop 1)


