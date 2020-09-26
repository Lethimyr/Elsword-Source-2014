CREATE PROCEDURE [dbo].[GGGA_game_game_update_buycount]
	@iLoginUID_ [int],
	@iType_ [tinyint],
	@iGGCount_ [tinyint],
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

if not exists
(
    select *
    from dbo.GGGAGameGameBuyCountData
    where
        LoginUID = @iLoginUID_
    and Type = @iType_
)
begin    select @iOK = -2    goto end_proc    end

begin transaction

update a
with (updlock)
set
    a.GGCount = a.GGCount + @iGGCount_
from dbo.GGGAGameGameBuyCountData
as a
with (updlock)
where
    LoginUID = @iLoginUID_
and Type = @iType_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


