CREATE PROCEDURE [dbo].[UID_user_item_drop]
	@strLogin_ [nvarchar](24),
	@iOK [int] = 0
AS
set nocount on

declare @LoginUID int

SELECT @LoginUID = LoginUID
FROM dbo.users
WITH (nolock)
WHERE
    Login = @strLogin_

if exists
(
    SELECT *
	FROM dbo.UAGUserAuth
	WITH (nolock)
	WHERE
	    LoginUID = @LoginUID
    and
        AuthLevel > 0
)
begin
    begin tran
        delete a
        with (updlock)
        FROM dbo.GoodsObjectList as a
        WITH (updlock)
        WHERE ownerlogin = @strLogin_

        if @@error <> 0
        begin select @iOK = -101 goto fail_tran end
        
        delete a
        with (updlock)
        FROM dbo.DurationItemObjectList as a
        WITH (updlock)
        WHERE ownerlogin = @strLogin_
        
        if @@error <> 0
        begin select @iOK = -102 goto fail_tran end
        
        delete a
        with (updlock)
		FROM dbo.GPet as a
		WITH (updlock)
		WHERE LoginUID = @LoginUID
        
        if @@error <> 0
        begin select @iOK = -103 goto fail_tran end
    commit tran
end

goto end_proc

fail_tran:
    rollback tran

end_proc:
    select @iOK


