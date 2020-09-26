CREATE PROCEDURE [dbo].[EPDB_event_perfect_party_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 파티 목록
*/

SELECT
    EventUID
,   PartyNum
,   EXPRatio
,   GPRatio
FROM dbo.EPGAEventPerfectParty
OPTION (MAXDOP 1)


