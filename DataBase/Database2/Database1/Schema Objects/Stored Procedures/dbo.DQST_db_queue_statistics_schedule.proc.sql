CREATE PROCEDURE [dbo].[DQST_db_queue_statistics_schedule]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081226. microcat.
*/

declare
    @sdtDateB smalldatetime
select
    @sdtDateB = getdate()

if exists
(
    select *
    from dbo.DQSTDbQueueTotal
    where
        RegDate = @sdtDateB
)
begin    goto end_proc    end

begin transaction

insert into dbo.DQSTDbQueueTotal
with (updlock)
(
    RegDate
,   QueueSize
)
select
    @sdtDateB
,   sum(QueueSize)
from dbo.DQSTDbQueue
with (updlock)
option (maxdop 1)

if @@error <> 0
begin    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:


