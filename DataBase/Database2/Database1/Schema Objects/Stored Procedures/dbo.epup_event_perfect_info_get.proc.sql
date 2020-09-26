CREATE PROCEDURE [dbo].[epup_event_perfect_info_get]
AS
set nocount on
set transaction isolation level read uncommitted

/*microcat
작성일 : 2007년 10월 4일
사용 : 퍼펙트한 이벤트의 값들을 리턴
*/

declare
    @sdtDate smalldatetime
select
    @sdtDate = getdate()

select
    EventUID
,   StartDate
,   EndDate
,   EventID
,   GPRatio
,   EXPRatio
,   ApplicationType
,   EquipItemA
,   EquipItemB
,   ActivityRate
,   CharType
,   Promotion
from dbo.EPEventPerfect
where
    StartDate <= @sdtDate
and @sdtDate < EndDate


