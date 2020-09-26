CREATE procedure dbo.EPGA_event_perfect_mode_select
    @iEventUID [int]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    이벤트 적용 모드 리스트
    
    { call dbo.EPGA_event_perfect_mode_select ( %d ) }
    { call dbo.EPGA_event_perfect_mode_select ( @1 ) }
    @1 ; iEventUID int
    
    n return ( @1, @2 )
    @1 ; ModeID int
    @2 ; Type int
*/

select
    ModeID
,   Type
from dbo.EPGAEventPerfectMode
where
    EventUID = @iEventUID


