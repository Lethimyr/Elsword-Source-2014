CREATE PROCEDURE [dbo].[UNGA_user_nickname_update]
	@iLoginUID_ [int],
	@strOldNickname_ [nvarchar](24),
	@strNewNickname_ [nvarchar](24),
	@iItemUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081219. microcat.

L"{ call dbo.UNGA_user_nickname_update ( %d, N'%s', N'%s', %d ) }"

{ call dbo.UNGA_user_nickname_update ( @1, @2, @3, @4 ) }
@1 ; LoginUID int
@2 ; OldNickname nvarchar(24)
@3 ; NewNickname nvarchar(24)
@4 ; ItemUID int ; 닉네임 변경카드

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; 닉네임 정보가 없음
    -3       ; 이미 존재하는 닉네임
    -4       ; 닉네임 변경카드가 아님
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
    from dbo.Nicknames
    where
        Login = @strLogin
    and Nick = @strOldNickname_
)
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.Nicknames
    where
        Nick = @strNewNickname_
)
begin    select @iOK = -2    goto end_proc    end

declare
    @strBuyerLogin nvarchar(20)
,   @sdtRegDate smalldatetime
select
    @strBuyerLogin = BuyerLogin
,   @sdtRegDate = RegDate
from dbo.GoodsObjectList
where
    ItemUID = @iItemUID_
and OwnerLogin = @strLogin
and ItemID = 23720
and Period = -1

if @strBuyerLogin is null
begin    select @iOK = -3    goto end_proc    end

declare
    @dtDate datetime
select
    @dtDate = current_timestamp

begin transaction

update a
with (updlock)
set
    Nick = @strNewNickname_
,   RegDate = @dtDate
from dbo.Nicknames
as a
with (updlock)
where
    Login = @strLogin
and Nick = @strOldNickname_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

execute dbo.UNMO_user_nickname_merge
    @iLoginUID_
,   @strNewNickname_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

insert into dbo.LogNicknames
with (updlock)
(
    Login
,   OldNick
,   NewNick
)
select
    @strLogin
,   @strOldNickname_
,   @strNewNickname_

if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

delete a
with (updlock)
from dbo.GoodsObjectList
as a
with (updlock)
where
    ItemUID = @iItemUID_
and OwnerLogin = @strLogin
and ItemID = 23720
and Period = -1

if @@error <> 0
begin    select @iOK = -104    goto fail_tran    end

insert into [Log].dbo.IPLOItemPeriodItem
with (updlock)
(
    LoginUID
,   ItemUID
,   BuyerLogin
,   ItemID
,   RegDate
,   DelDate
,   StartDate
,   EndDate
,   Period
,   Expired
)
select
    @iLoginUID_
,   @iItemUID_
,   @strBuyerLogin
,   23720
,   @sdtRegDate
,   @dtDate
,   @sdtRegDate
,   dateadd(dd, -1, @sdtRegDate)
,   -1
,   0

if @@error <> 0
begin    select @iOK = -105    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


