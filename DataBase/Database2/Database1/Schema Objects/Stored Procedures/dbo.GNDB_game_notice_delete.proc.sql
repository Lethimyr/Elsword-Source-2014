CREATE PROCEDURE [dbo].[GNDB_game_notice_delete]
	@iNo_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081010. microcat.
*/

if not exists
(
    select *
    from dbo.GNGAGameNotice
    where
        No = @iNo_
)
begin    select @iOK = -1    goto end_proc    end

begin transaction

delete a
with (updlock)
from dbo.GNGAGameNotice
as a
with (updlock)
where
    No = @iNo_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


