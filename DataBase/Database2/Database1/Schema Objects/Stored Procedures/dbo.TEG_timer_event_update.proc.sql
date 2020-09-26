CREATE PROCEDURE [dbo].[TEG_timer_event_update]
	@iIssueUID_ [int],
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

if not exists
(
    select *
    from dbo.TEGTimerEventDate
    where
        IssueUID = @iIssueUID_
    and Status = 2
)
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.TELTimerEvent
    where
        IssueUID = @iIssueUID_
)
begin    select @iOK = -2    goto end_proc    end

begin tran
    update a
    with (updlock)
    set
        a.Status = 3
    from dbo.TEGTimerEventDate
    as a
    with (updlock)
    where
        IssueUID = @iIssueUID_
    and Status = 2
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    insert into dbo.TELTimerEvent
    with (updlock)
    (
        IssueUID
    ,   LoginUID
    )
    select
        @iIssueUID_
    ,   @iLoginUID_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


