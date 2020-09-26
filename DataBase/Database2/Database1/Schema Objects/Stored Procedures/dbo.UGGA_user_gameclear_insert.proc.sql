CREATE PROCEDURE [dbo].[UGGA_user_gameclear_insert]
	@iLoginUID_ [int],
	@iModeID_ [smallint],
	@iDifficulty_ [smallint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081107. microcat.

{ call dbo.UGGA_user_gameclear_insert ( %d, %d, %d ) }

{ call dbo.UGGA_user_gameclear_insert ( @1, @2, @3 ) }
@1 ; LoginUID int
@2 ; ModeID smallint
@3 ; Difficulty smallint

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 유저가 존재하지 않음
-2 ; 게임클리어 정보가 이미 존재함
-101이하 ; 트랜잭션 에러
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
    from dbo.UGGAUserGameClear
    where
        LoginUID = @iLoginUID_
    and ModeID = @iModeID_
    and Difficulty = @iDifficulty_
)
begin    select @iOK = -2    goto end_proc    end

--/*
--부연설명
--아래 쿼리는 2008년 11월 15일에 작성되었으며
--약 1년 후에 이 쿼리는 삭제되어야 합니다.
if exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
    and len(ModeLevel) > 4
)
begin
    begin transaction

    update a
    with (updlock)
    set
        ModeLevel = 0
    from dbo.Users
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    commit transaction
end
--*/

begin transaction

insert into dbo.UGGAUserGameClear
with (updlock)
(
    LoginUID
,   ModeID
,   Difficulty
,   RegDateA
)
select
    @iLoginUID_
,   @iModeID_
,   @iDifficulty_
,   current_timestamp
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


