CREATE PROCEDURE [dbo].[EPDB_event_perfect_character_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 목록
*/

SELECT
    EventUID
,   CharType
,   Promotion 
FROM dbo.EPGAEventPerfectCharacter
OPTION (MAXDOP 1)


