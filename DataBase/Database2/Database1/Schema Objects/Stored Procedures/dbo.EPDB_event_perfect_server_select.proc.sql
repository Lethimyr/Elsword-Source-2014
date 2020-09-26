CREATE PROCEDURE [dbo].[EPDB_event_perfect_server_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 서버 목록
*/

SELECT
    EventUID
,   ServerPart
FROM dbo.EPGAEventPerfectServer
OPTION (MAXDOP 1)


