CREATE PROCEDURE [dbo].[TEG_timer_event_date_select]
AS
set nocount on
set transaction isolation level read uncommitted

declare
    @sdtDateB smalldatetime
select
    @sdtDateB = getdate()

declare
    @data table
(
    IssueUID int not null
,   MessageID int not null
)
insert into @data
(
    IssueUID
,   MessageID
)
select
    IssueUID
,   MessageID
from dbo.TEGTimerEventDate
where
    RegDateB <= @sdtDateB
and @sdtDateB < dateadd(mi, IntervalM, RegDateB)
and Status = 1
option (maxdop 1)
if @@error <> 0
begin    goto end_proc    end

if not exists
(
    select *
    from @data
)
begin    goto end_proc    end

begin tran
    update a
    with (updlock)
    set
        a.Status = 2
    from dbo.TEGTimerEventDate
    as a
    with (updlock)
    join @data
    as b
    on
        a.IssueUID = b.IssueUID
    and a.Status = 1
    option (maxdop 1)
    if @@error <> 0
    begin    goto fail_tran    end
commit tran

fail_tran:
rollback tran

end_proc:
select
    IssueUID
,   MessageID
from @data
option (maxdop 1)


