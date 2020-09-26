CREATE PROCEDURE [dbo].[up_get_exist_user_in_agreement]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

if not exists
(
    select *
    from dbo.Agreement
    where
        Login = @strLogin_
)
begin    select @iOK = -1    goto end_proc    end

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


