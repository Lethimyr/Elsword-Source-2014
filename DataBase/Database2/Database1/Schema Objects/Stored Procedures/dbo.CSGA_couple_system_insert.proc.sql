CREATE PROCEDURE [dbo].[CSGA_couple_system_insert]
	@iLoginUID_ [int],
	@iLoginUIDE_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_insert ( %d, %d ) }

{ call dbo.CSGA_couple_system_insert ( @1, @2 ) }
@1 ; LoginUID int ; 내꺼
@2 ; LoginUIDE int ; 커플꺼

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-2 ; 커플꺼 유저 정보가 존재하지 않음
-3 ; 내가 이미 커플 상태에 있음
-4 ; 커플꺼가 이미 커플 상태에 있음
-101이하 ; 트랜잭션 에러
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

execute dbo.AAA_login_check
    @iLoginUIDE_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.CSGACoupleSystem
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -3    goto end_proc    end

if exists
(
    select *
    from dbo.CSGACoupleSystem
    where
        LoginUID = @iLoginUIDE_
)
begin    select @iOK = -4    goto end_proc    end

declare
    @dtDateA datetime
select
    @dtDateA = current_timestamp

begin transaction

insert into dbo.CSGACoupleSystem
with (updlock)
(
    LoginUID
,   LoginUIDE
,   RegDateA
)
select
    @iLoginUID_
,   @iLoginUIDE_
,   @dtDateA
union all
select
    @iLoginUIDE_
,   @iLoginUID_
,   dateadd(mi, 1, @dtDateA)
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


