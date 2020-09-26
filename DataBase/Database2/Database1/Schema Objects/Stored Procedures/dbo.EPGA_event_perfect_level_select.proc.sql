create procedure dbo.EPGA_event_perfect_level_select
    @iEventUID [int]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    이벤트 적용 레벨 리스트
    
    { call dbo.EPGA_event_perfect_level_select ( %d ) }
    { call dbo.EPGA_event_perfect_level_select ( @1 ) }
    @1 ; iEventUID int
    
    n return ( @1, @2 )
    @1 ; StartLevel int
    @2 ; EndLevel int
*/

select
    StartLevel
,   EndLevel
from dbo.EPGAEventPerfectLevel
where
    EventUID = @iEventUID


