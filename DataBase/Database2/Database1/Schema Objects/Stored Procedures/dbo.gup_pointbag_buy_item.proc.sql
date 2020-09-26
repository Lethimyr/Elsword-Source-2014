CREATE PROCEDURE [dbo].[gup_pointbag_buy_item]
	@iLoginUID_ [int],
	@iItemID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 25일
사용 : 포인트를 가지고 아이템을 구매
*/
SET NOCOUNT ON;

exec @iOK = dbo.gup_pointbag_buy_item_available @iLoginUID_, @iItemID_, 0

if @iOK <> 0
    begin    return    end

declare @strLogin nvarchar(20)
,       @strSQL nvarchar(4000)
,       @iPrice int
select  @strLogin = Login
,       @strSQL = N''
,       @iPrice = 0
    from dbo.Users with (nolock)
        where LoginUID = @iLoginUID_

declare @iFactor int
select  @iFactor = Factor
,       @iPrice = Price
    from dbo.GPointBagItemList with (nolock)
        where ItemID = @iItemID_

if exists(select * from dbo.GPointBagItemPart with (nolock) where ItemID = @iItemID_)
    begin
        select  @strSQL = @strSQL + cast(SubItemID as nvarchar(10)) + N',' + cast(@iFactor as nvarchar(10)) + N','
            from dbo.GPointBagItemPart with (nolock) where ItemID = @iitemID_
    end
else
    begin
        select  @strSQL = @strSQL + cast(@iItemID_ as nvarchar(10)) + N',' + cast(@iFactor as nvarchar(10)) + N','
    end

begin tran
    update dbo.GPointBag with (updlock)
        set Point = Point - @iPrice
            where LoginUID = @iLoginUID_

    insert into dbo.GPointBagItemLog
            (
                LoginUID
,               ItemID
,               Factor
,               Price
,               RegDate
            )
        select  @iLoginUID_
,               @iItemID_
,               @iFactor
,               @iPrice
,               GetDate()
    
    exec dbo.up_insert_item @strLogin, @strSQL
commit tran


