CREATE PROCEDURE [dbo].[RCR_rank_character_collect]
	@iLoginUID_ [int],
	@iCharType_ [tinyint],
	@iWin_ [int],
	@iLose_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081223. microcat.
*/

if exists
(
    select *
    from dbo.RCRRankCharacterCollect
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        Win = Win + case when @iWin_ < 0 then 0 else @iWin_ end
    ,   Lose = Lose + case when @iLose_ < 0 then 0 else @iLose_ end
    from dbo.RCRRankCharacterCollect
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.RCRRankCharacterCollect
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   Win
    ,   Lose
    )
    select
        @iLoginUID_
    ,   @iCharType_
    ,   @iWin_
    ,   @iLose_

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
return
    @iOK


