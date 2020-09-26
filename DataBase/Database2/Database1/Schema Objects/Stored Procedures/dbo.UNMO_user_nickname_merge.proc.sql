CREATE PROCEDURE [dbo].[UNMO_user_nickname_merge]
	@iLoginUID_ [int],
	@strNickname_ [nvarchar](24),
	@iOK [int] = null OUTPUT
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081219. microcat.
*/

if @@nestlevel < 2
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.UNGAUserNickname
    where
        LoginUID = @iLoginUID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        Nickname = @strNickname_
    from dbo.UNGAUserNickname
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.UNGAUserNickname
    with (updlock)
    (
        LoginUID
    ,   Nickname
    )
    select
        @iLoginUID_
    ,   @strNickname_

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:


