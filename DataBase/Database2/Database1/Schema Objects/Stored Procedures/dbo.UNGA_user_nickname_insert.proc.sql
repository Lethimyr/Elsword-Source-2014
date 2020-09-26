CREATE PROCEDURE [dbo].[UNGA_user_nickname_insert]
	@iLoginUID_ [int],
	@strNickname_ [nvarchar](24),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081229. microcat.

L"{ call dbo.UNGA_user_nickname_insert ( %d, N'%s' ) }"

{ call dbo.UNGA_user_nickname_insert ( @1, @2 ) }
@1 ; LoginUID int
@2 ; Nickname nvarchar(24)

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; 닉네임 정보가 이미 있음
    -3       ; 이미 존재하는 닉네임
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

if exists
(
    select *
    from dbo.Nicknames
    where
        Login = @strLogin
)
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.Nicknames
    where
        Nick = @strNickname_
)
begin    select @iOK = -2    goto end_proc    end

begin transaction

insert into dbo.NickNames
with (updlock)
(
    Login
,   Nick
)
select
    @strLogin
,   @strNickName_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

execute dbo.UNMO_user_nickname_merge
    @iLoginUID_
,   @strNickname_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


