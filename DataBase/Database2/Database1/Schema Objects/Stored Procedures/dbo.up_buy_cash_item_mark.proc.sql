CREATE PROCEDURE [dbo].[up_buy_cash_item_mark]
	@iItemUID_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
set nocount on;

if not exists(select * from dbo.CashItemDisplayList with (nolock) where ItemUID = @iItemUID_)
    begin    select @iOK = -1    goto end_proc    end

begin tran
    update a with (updlock)
        set a.FromWeb = 2
            from dbo.CashItemDisplayList as a with (updlock)
                where a.ItemUID = @iItemUID_
    if @@error <> 0
    begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
if @iOK <> 0
    begin
        declare @strDB nvarchar(4000)
,               @strProc nvarchar(4000)
        select  @strDB = db_name()
,               @strProc = object_name(@@procid)
        raiserror('microcat : %s.dbo.%s(OK : %d, Error : %d, ItemUID : %d)', 16, 127, @strDB, @strProc, @iOK, @iError, @iItemUID_)
    end
select @iOK


