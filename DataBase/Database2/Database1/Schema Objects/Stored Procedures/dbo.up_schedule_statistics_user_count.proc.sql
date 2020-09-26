CREATE PROCEDURE [dbo].[up_schedule_statistics_user_count]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081020. microcat.
*/

declare
    @sdtDateB smalldatetime
select
    @sdtDateB = getdate()

if exists
(
    select *
    from dbo.TotalConnectUsers
    where
        [Date] = @sdtDateB
)
begin    goto end_proc    end

begin transaction

insert into dbo.TotalConnectUsers
with (updlock)
(
    [Date]
,   NumUsers
)
select
    @sdtDateB
,   sum(UserNum)
from dbo.ConnectStatusDB
with (updlock)
option (maxdop 1)

if @@error <> 0
begin    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:


