create PROCEDURE [dbo].[EPDB_event_perfect_mode_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 모드 목록
*/

SELECT
    EventUID
,   ModeID
,   Type
FROM dbo.EPGAEventPerfectMode
OPTION (MAXDOP 1)


