CREATE PROCEDURE [dbo].[EPDB_event_perfect_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 목록
*/

SELECT
    EventUID
,   EventID
,   convert(nvarchar(16), StartDate, 20) as StartDate
,   convert(nvarchar(16), EndDate, 20) as EndDate
,   ApplicationType
,   ActivityRate
,   EXPRatio
,   GPRatio
FROM dbo.EPGAEventPerfect
OPTION (MAXDOP 1)


