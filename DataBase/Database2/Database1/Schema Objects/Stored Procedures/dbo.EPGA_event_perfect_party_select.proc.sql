CREATE PROCEDURE [dbo].[EPGA_event_perfect_party_select]
	@iEventUID [int]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    이벤트 파티원 보상
    
    { call dbo.EPGA_event_perfect_party_select ( %d ) }
    { call dbo.EPGA_event_perfect_party_select ( @1 ) }
    @1 ; iEventUID int
    
    n return ( @1, @2, @3 )
    @1 ; PartyNum int
    @2 ; GPRatio int
    @3 ; EXPRatio int
*/

select
    PartyNum
,   GPRatio
,   EXPRatio
from dbo.EPGAEventPerfectParty
where
    EventUID = @iEventUID


