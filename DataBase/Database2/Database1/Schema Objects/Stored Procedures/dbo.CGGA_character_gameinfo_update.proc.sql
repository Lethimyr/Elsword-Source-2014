CREATE PROCEDURE [dbo].[CGGA_character_gameinfo_update]
	@iLoginUID_ [int],
	@iCharType_ [tinyint],
	@iExp_ [bigint],
	@iWin_ [int],
	@iLose_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081229. microcat.

L"{ call dbo.CGGA_character_gameinfo_update ( %d, %d, %d, %d, %d ) }"

{ call dbo.CGGA_character_gameinfo_update ( @1, @2, @3, @4, @5 ) }
@1 ; @iLoginUID_ int
@2 ; @iCharType_ tinyint
    0 ; 엘리시스
    1 ; 리르
    2 ; 아르메
    3 ; 라스
    4 ; 라이언
    5 ; 로난
    6 ; 에이미
    7 ; 진
@3 ; @iExp_ bigint  ; 차이값만 주쇼
@4 ; @iWin_ int     ; 차이값만 주쇼
@5 ; @iLose_ int    ; 차이값만 주쇼

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; 캐릭터가 없음
    -101이하 ; 트랜잭션 에러
*/

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_

if @strLogin is null
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.Characters
    where
        Login = @strLogin
    and CharType = @iCharType_
)
begin    select @iOK = -2    goto end_proc    end

begin transaction

update a
with (updlock)
set
    [Exp] = [Exp] + @iExp_
,   Level = dbo.zbLevel_Exp([Exp] + @iExp_)
,   Win = Win + @iWin_
,   Lose = Lose + @iLose_
from dbo.Characters
as a
with (updlock)
where
    Login = @strLogin
and CharType = @iCharType_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

if @iWin_ + @iLose_ > 0
begin
    execute @iOK = dbo.RCR_rank_character_collect
        @iLoginUID_
    ,   @iCharType_
    ,   @iWin_
    ,   @iLose_

    if @iOK <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


