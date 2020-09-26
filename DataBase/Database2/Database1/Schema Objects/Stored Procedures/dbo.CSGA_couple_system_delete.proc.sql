CREATE PROCEDURE [dbo].[CSGA_couple_system_delete]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.
아... 변태가 되었음.
게임서버에서 호출하는 것 이외에도
up_drop_user_info sp에서 호출하는 것도 포함되었음

{ call dbo.CSGA_couple_system_delete ( %d ) }

{ call dbo.CSGA_couple_system_delete ( @1 ) }
@1 ; LoginUID int ; 내꺼

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-2 ; 내 커플 정보가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

declare
    @iLoginUIDE int
,   @dtRegDateA datetime
select
    @iLoginUIDE = LoginUIDE
,   @dtRegDateA = RegDateA
from dbo.CSGACoupleSystem
where
    LoginUID = @iLoginUID_
option (maxdop 1)

if @iLoginUIDE is null
begin    select @iOK = -2    goto end_proc    end

begin transaction

delete a
with (updlock)
from dbo.CSGACoupleSystem
as a
with (updlock)
where
    LoginUID = @iLoginUID_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

execute [Log].dbo.CSMO_couple_system_insert
    @iLoginUID_
,   @iLoginUIDE
,   @dtREgDateA
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
if @@nestlevel < 2
begin
    select
        @iOK
end
else
begin
    return
        @iOK
end


