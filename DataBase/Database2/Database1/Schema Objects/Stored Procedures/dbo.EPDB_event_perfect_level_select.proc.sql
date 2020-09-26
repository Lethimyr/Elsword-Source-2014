CREATE PROCEDURE dbo.EPDB_event_perfect_level_select
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 레벨 목록
*/

SELECT
    EventUID
,   StartLevel
,   EndLevel 
FROM dbo.EPGAEventPerfectLevel
OPTION (MAXDOP 1)


