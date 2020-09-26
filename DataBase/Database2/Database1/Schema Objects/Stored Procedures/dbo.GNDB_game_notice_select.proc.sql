CREATE PROCEDURE [dbo].[GNDB_game_notice_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081010. microcat.
*/

select
    No
,   convert(nvarchar(16), StartDateB, 120) as StartDateB
,   convert(nvarchar(16), EndDateB, 120) as EndDateB
,   case
        when current_timestamp between StartDateB and EndDateB then N'Progress'
        else N'...'
    end as Status
,   Message
,   IntervalM
from dbo.GNGAGameNotice
option (maxdop 1)


