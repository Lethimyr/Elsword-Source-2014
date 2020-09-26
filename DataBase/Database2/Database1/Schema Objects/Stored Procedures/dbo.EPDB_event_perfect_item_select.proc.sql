CREATE PROCEDURE [dbo].[EPDB_event_perfect_item_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 아이템 목록
*/

SELECT
    EventUID
,   Type
,   ItemID 
FROM dbo.EPGAEventPerfectItem
OPTION (MAXDOP 1)


