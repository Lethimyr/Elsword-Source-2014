CREATE PROCEDURE [dbo].[CSDB_check_sum_delete]
	@iType_ [tinyint],
	@iProtocol_ [int],
	@binCheckSum_ [binary](4),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090105. microcat.
*/

if not exists
(
    select *
    from dbo.CSGACheckSum
    where
        Type = @iType_
    and Protocol = @iProtocol_
    and [CheckSum] = @binCheckSum_
)
begin    select @iOK = -1    goto end_proc    end

begin transaction

delete a
with (updlock)
from dbo.CSGACheckSum
as a
with (updlock)
where
    Type = @iType_
and Protocol = @iProtocol_
and [CheckSum] = @binCheckSum_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


