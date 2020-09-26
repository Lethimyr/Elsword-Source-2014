CREATE PROCEDURE [dbo].[MGG_mini_game_select]
	@iLoginUID_ [int]
AS
set nocount on

if exists
(
    select *
    from dbo.MGGMiniGame
    with (nolock)
    where
        LoginUID = @iLoginUID_
)
begin    goto end_proc    end

begin tran
    insert into dbo.MGGMiniGame
    with (updlock)
    (
        LoginUID
    ,   GameMode
    ,   Chartype
    ,   Point
    ,   PlayTime
    )
    select
        @iLoginUID_
    ,   No
    ,   No
    ,   0
    ,   0
    from
    (
        select
            0 as No
        union all
        select
            1
    )
    as a
    option (maxdop 1)
    if @@error <> 0
    begin    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    GameMode
,   CharType
,   Point
from dbo.MGGMiniGame
with (nolock)
where
    LoginUID = @iLoginUID_


