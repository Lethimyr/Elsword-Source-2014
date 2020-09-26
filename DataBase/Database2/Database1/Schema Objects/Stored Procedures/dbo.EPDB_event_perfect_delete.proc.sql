CREATE PROCEDURE [dbo].[EPDB_event_perfect_delete]
	@iEventUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
    20090312. mk8253.
    이벤트 삭제
*/

if not exists
(
    select *
    from dbo.EPGAEventPerfect
    where EventUID = @iEventUID_
)
begin    select @iOK = -1    goto end_proc    end

begin transaction

delete a
with (updlock)
from dbo.EPGAEventPerfect as a
with (updlock)
where EventUID = @iEventUID_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


