CREATE PROCEDURE [dbo].[MGG_mini_game_update]
	@iLoginUID_ [int],
	@iGameMode_ [tinyint],
	@iCharType_ [tinyint],
	@iPoint_ [int],
	@iPlayTime_ [int],
	@iOK [int] = 0
AS
set nocount on

if not exists
(
    select *
    from dbo.MGGMiniGame
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and GameMode = @iGameMode_
    and Point < @iPoint_
)
begin    select @iOK = -1    goto end_proc    end

begin tran
    update a
    with (updlock)
    set
        a.CharType = @iCharType_
    ,   a.Point = @iPoint_
    ,   a.PlayTime = @iPlayTime_
    from dbo.MGGMiniGame
    as a
    with (updlock)
        where
            LoginUID = @iLoginUID_
        and GameMode = @iGameMode_
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


