CREATE procedure dbo.EPGA_event_perfect_server_select
    @iEventUID [int]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    이벤트 서버 리스트
    
    { call dbo.EPGA_event_perfect_server_select ( %d ) }
    { call dbo.EPGA_event_perfect_server_select ( @1 ) }
    @1 ; iEventUID int
    
    n return ( @1 )
    @1 ; ServerPart int
*/

select
    ServerPart
from dbo.EPGAEventPerfectServer
where
    EventUID = @iEventUID


