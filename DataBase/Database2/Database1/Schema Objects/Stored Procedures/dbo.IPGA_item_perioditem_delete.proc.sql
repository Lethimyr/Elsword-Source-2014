CREATE PROCEDURE [dbo].[IPGA_item_perioditem_delete]
	@iLoginUID_ [int],
	@iItemUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081220. microcat.

L"{ call dbo.IPGA_item_perioditem_delete ( %d, %d ) }"

{ call dbo.IPGA_item_perioditem_delete ( @1, @2 ) }
@1 ; @iLoginUID_ [int],
@1 ; @iItemUID_ [int],

1 return ( @1 )
@1 ; OK [int]
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; 아이템 정보가 없음
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

declare
    @dtDateA datetime
,   @strBuyerLogin nvarchar(20)
,   @iItemID int
,   @sdtRegDate smalldatetime
,   @sdtStartDate smalldatetime
,   @sdtEndDate smalldatetime
,   @iPeriod int
select
    @dtDateA = current_timestamp

select
    @strBuyerLogin = BuyerLogin
,   @iItemID = ItemID
,   @sdtRegDate = RegDate
,   @sdtStartDate = StartDate
,   @sdtEndDate = EndDate
,   @iPeriod = Period
from dbo.GoodsObjectList
where
    OwnerLogin = @strLogin
and RegDate <= StartDate
and EndDate < @dtDateA
and Period > 0
and ItemUID = @iItemUID_

if @strBuyerLogin is null
begin    select @iOK = -2    goto end_proc    end

begin transaction

delete a
with (updlock)
from dbo.GoodsObjectList
as a
with (updlock)
where
    OwnerLogin = @strLogin
and RegDate <= StartDate
and EndDate < @dtDateA
and Period > 0
and ItemUID = @iItemUID_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

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
,   @iItemID
,   @sdtRegDate
,   @dtDateA
,   @sdtStartDate
,   @sdtEndDate
,   @iPeriod
,   1

if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


