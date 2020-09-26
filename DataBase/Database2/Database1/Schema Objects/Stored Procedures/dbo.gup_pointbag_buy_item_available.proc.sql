CREATE PROCEDURE [dbo].[gup_pointbag_buy_item_available]
	@iLoginUID_ [int],
	@iItemID_ [int],
	@bSelectReturn [bit] = 1,
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 25일
사용 : 포인트를 가지고 아이템을 구매할 수 있는지 체크
*/
set nocount on;

if not exists(select * from dbo.Users with (nolock) where LoginUID = @iLoginUID_)
    begin    select @iOK = -1    goto end_proc    end

if not exists(select * from dbo.GoodsInfoList with (nolock) where GoodsID = @iItemID_)
    begin    select @iOK = -2    goto end_proc    end

declare @iPoint int
select  @iPoint = Point
    from dbo.GPointBag with (nolock)
        where LoginUID = @iLoginUID_
if @iPoint is null
    begin    select @iOK = -3    goto end_proc    end

declare @iFactor int
,       @bSaled bit
,       @iPrice int
select  @iPrice = 0
select  @iFactor = Factor
,       @bSaled = Saled
,       @iPrice = Price
    from dbo.GPointBagItemList with (nolock)
        where ItemID = @iItemID_
if @iFactor is null
    begin    select @iOK = -4    goto end_proc    end

if @bSaled = 0 --판매중이 아니라면
    begin    select @iOK = -5    goto end_proc    end

if @iPoint < @iPrice --쥐뿔 포인트도 안되면서
    begin    select @iOK = -6    goto end_proc    end

end_proc:
if @bSelectReturn = 1
    begin
        select @iOK, @iPrice
    end
else if @bSelectReturn = 0
    begin
        return @iOK
    end


