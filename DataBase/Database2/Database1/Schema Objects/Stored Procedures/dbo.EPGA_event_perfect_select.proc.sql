CREATE PROCEDURE [dbo].[EPGA_event_perfect_select]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    진행중인 이벤트 리스트
    
    { call dbo.EPGA_event_perfect_select }
    
    n return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1 ; EventUID int
    @2 ; EventID int
    @3 ; StartDate int
    @4 ; EndDate int
    @5 ; ApplicationType int
    @6 ; ActivityRate int
    @7 ; GPRatio int
    @8 ; EXPRatio int
*/
declare  
    @sdtDate smalldatetime  
select  
    @sdtDate = getdate()  
  
select  
    EventUID
,   EventID
,   StartDate
,   EndDate
,   ApplicationType
,   ActivityRate
,   GPRatio
,   EXPRatio 
from dbo.EPGAEventPerfect  
where  
    StartDate <= @sdtDate  
and @sdtDate < EndDate


