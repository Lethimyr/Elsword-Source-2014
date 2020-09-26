CREATE PROCEDURE [dbo].[up_dbot_reset_account]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
with (nolock)
where
    Login = @strLogin_
if @iLoginUID is null
begin    select @iOK = -1    goto fail_tran    end

begin tran
    update a
    with (updlock)
    set
        a.GamePoint = 200
    from dbo.Users
    as a
    with (updlock)
    where
        Login = @strLogin_
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    update a
    with (updlock)
    set
        Promotion = 0
    ,   [Exp] = 0
    ,   Level = 0
    ,   Win = 0
    ,   Lose = 0
    from dbo.Characters
    as a
    with (updlock)
    where
        Login = @strLogin_
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    delete a
    with (updlock)
    from dbo.STGSkillTreeID
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID
    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end

    delete a
    with (updlock)
    from dbo.STGSkillTreeSet
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID
    if @@error <> 0
    begin    select @iOK = -104    goto fail_tran    end

    delete a
    with (updlock)
    from dbo.STGSkillTreeSetEquip
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID
    if @@error <> 0
    begin    select @iOK = -105    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


