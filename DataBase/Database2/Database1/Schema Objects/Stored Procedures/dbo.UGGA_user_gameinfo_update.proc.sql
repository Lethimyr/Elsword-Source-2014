CREATE PROCEDURE [dbo].[UGGA_user_gameinfo_update]
	@iLoginUID_ [int],
	@iGamePointDiff_ [int],
	@iOK [int] = 0,
	@iGamePointTotal [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081107. microcat.

{ call dbo.UGGA_user_gameinfo_update ( %d, %d ) }

{ call dbo.UGGA_user_gameinfo_update ( @1, @2 ) }
@1 ; LoginUID int
@2 ; GamePointDiff int

1 return ( @1, @2 )
@1 ; OK int
@2 ; GamePointTotal int

OK
0 ; 성공
-1 ; 유저가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

declare
    @iGamePoint int
select
    @iGamePoint = GamePoint
from dbo.Users
where
    LoginUID = @iLoginUID_
option (maxdop 1)

if @iGamePoint is null
begin    select @iOK = -1    goto end_proc    end

select
    @iGamePointTotal = dbo.zcGamePoint(@iGamePoint + @iGamePointDiff_)

begin transaction

update a
with (updlock)
set
    a.LastLogin = dateadd(mi, 1, current_timestamp)
,   a.GamePoint = @iGamePointTotal
from dbo.Users
as a
with (updlock)
where
    LoginUID = @iLoginUID_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

execute Stat.dbo.LLMO_last_login_insert
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

execute [Log].dbo.GPMO_game_point_insert
    @iLoginUID_
,   @iGamePoint
,   @iGamePointDiff_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -103    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK
,   @iGamePointTotal


