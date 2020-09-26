CREATE PROCEDURE [dbo].[epup_event_perfect_mode_get]
	@iEventUID_ [int]
AS
/*microcat
작성일 : 2007년 10월 4일
사용 : 퍼펙트한 이벤트의 모드값을 리턴
*/
set nocount on;

select  ModeID
    from dbo.EPEventPerfectMode with (nolock)
        where EventUID = @iEventUID_


