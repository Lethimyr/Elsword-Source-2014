CREATE PROCEDURE [dbo].[GGG_game_game_user_insert]
	@iLoginUID_ [int],
	@iItemID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080911. microcat.
*/

if not exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.GGGGameGameUser
    where
        LoginUID = @iLoginUID_
    and ItemID = @iItemID_
)
begin    select @iOK = -2    goto end_proc    end

begin tran
    insert into dbo.GGGGameGameUser
    with (updlock)
    (
        LoginUID
    ,   ItemID
    )
    select
        @iLoginUID_
    ,   @iItemID_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


