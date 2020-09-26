CREATE PROCEDURE [dbo].[CSDB_check_sum_insert]
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

if exists
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

insert into dbo.CSGACheckSum
with (updlock)
(
    Type
,   Protocol
,   [CheckSum]
,   RegDateA
)
select
    @iType_
,   @iProtocol_
,   @binCheckSum_
,   current_timestamp

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


