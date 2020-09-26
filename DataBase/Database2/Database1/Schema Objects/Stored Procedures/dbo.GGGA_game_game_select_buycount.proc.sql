CREATE PROCEDURE [dbo].[GGGA_game_game_select_buycount]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081020. microcat.
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

declare
    @sdtDateD smalldatetime
select
    @sdtDateD = convert(nvarchar(10), current_timestamp, 120)

if not exists
(
    select *
    from dbo.GGGAGameGameBuyCount
    where
        LoginUID = @iLoginUID_
)
begin
    begin transaction

    insert into dbo.GGGAGameGameBuyCount
    with (updlock)
    (
        LoginUID
    ,   RegDateD
    )
    select
        @iLoginUID_
    ,   @sdtDateD
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    insert into dbo.GGGAGameGameBuyCountData
    with (updlock)
    (
        LoginUID
    ,   Type
    ,   GGCount
    )
    select
        @iLoginUID_
    ,   a.Type
    ,   0
    from
    (
        select --Cash
            1 as Type
        union all
        select --GP
            2
    )
    as a
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    commit transaction
end

if exists
(
    select *
    from dbo.GGGAGameGameBuyCount
    where
        LoginUID = @iLoginUID_
    and RegDateD <> @sdtDateD
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.RegDateD = @sdtDateD
    from dbo.GGGAGameGameBuyCount
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    update a
    with (updlock)
    set
        a.GGCount = 0
    from dbo.GGGAGameGameBuyCountData
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    commit transaction
end

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    Type
,   GGCount
from dbo.GGGAGameGameBuyCountData
where
    LoginUID = @iLoginUID_
option (maxdop 1)


