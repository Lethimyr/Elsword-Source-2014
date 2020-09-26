CREATE PROCEDURE [dbo].[SCTE_snow_coin_insert]
	@iLoginUID_ [int],
	@iCount_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081208. microcat.

L"{ call dbo.SCTE_snow_coin_insert ( %d, %d ) }"

{ call dbo.SCTE_snow_coin_insert ( @1, @2 ) }
@1 ; LoginUID int
@2 ; Count int ; 획득한 코인의 개수

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 계정 정보가 존재하지 않음
    -2       ; Count가 1보다 작음
    -101이하 ; 트랜잭션 에러
*/

declare
    @strLogin nvarchar(12)
,   @strPassword nvarchar(12)
select
    @strLogin = Login
,   @strPassword = Passwd
from dbo.Users
where
    LoginUID = @iLoginUID_

if @strLogin is null
begin    select @iOK = -1    goto end_proc    end

if @iCount_ < 1
begin    select @iOK = -2    goto end_proc    end

begin transaction

insert into dbo.tbl_Event_SnowCoinCollected
with (updlock)
(
    UniID
,   UserID
)
select
    @strPassword
,   @strLogin
from dbo.TDNDataNo
with (updlock)
where
    No between 1 and @iCount_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


