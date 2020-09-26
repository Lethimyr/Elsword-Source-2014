CREATE PROCEDURE [dbo].[TEG_timer_event_item_select]
	@iIssueUID_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

select
    ItemID
,   Factor
,   GoodsName
from dbo.TEGTimerEventItem as a
join dbo.GoodsInfoList as b
on a.ItemID = b.GoodsID
where
    IssueUID = @iIssueUID_
option (maxdop 1)


