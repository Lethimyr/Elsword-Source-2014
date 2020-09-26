create procedure dbo.EPGA_event_perfect_character_select
    @iEventUID [int]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    이벤트의 적용 캐릭터 리스트
    
    { call dbo.EPGA_event_perfect_character_select ( %d ) }
    { call dbo.EPGA_event_perfect_character_select ( @1 ) }
    @1 ; iEventUID int
    
    n return ( @1, @2 )
    @1 ; CharType int
    @2 ; Promotion int
*/

select
    CharType
,   Promotion
from dbo.EPGAEventPerfectCharacter
where
    EventUID = @iEventUID


