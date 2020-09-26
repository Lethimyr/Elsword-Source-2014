CREATE PROCEDURE [dbo].[BIGA_buy_item_merge_GP]
	@iLoginUID_ [int],
	@iItemID_ [int],
	@iOK [int] = 0,
	@iGamePoint [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081218. microcat.

L"{ call dbo.BIGA_buy_item_merge_GP ( %d, %d ) }"

{ call dbo.BIGA_buy_item_merge_GP ( @1, @2 ) }
@2 ; LoginUID int
@3 ; ItemID int

1 return ( @1, @2, @3, @4, @5, @6, @7 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저가 존재하지 않음
    -2       ; 해당 아이템이 존재하지 않음
    -101이하 ; 트랜잭션 에러
@2 ; GamePoint int
@3 ; 빈데이터
@4 ; 빈데이터
@5 ; 빈데이터
@6 ; 빈데이터
@7 ; 빈데이터
union all
n return ( @1, @2, @3, @4, @5, @6, @7 )
@1 ; ItemType bit
    0 ; 영구기간
    1 ; 수량
@2 ; ItemID int
@3 ; ItemUID int
@4 ; Factor int
@5 ; RegDate smalldatetime
@6 ; StartDate smalldatetime
@7 ; EndDate smalldatetime
*/

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
,   @iGamePoint = GamePoint
from dbo.Users
where
    LoginUID = @iLoginUID_

if @strLogin is null
begin    select @iOK = -1    goto end_proc    end

declare
    @bSetID bit
,   @iSetID int
,   @bItemType bit
,   @iFactor int
,   @iPrice int
select
    @bSetID = IsSubSetGoods
,   @iSetID = SetGoodsNum
,   @bItemType = ItemType
,   @iFactor = Duration
,   @iPrice = Price
from dbo.GoodsInfoList
where
    GoodsID = @iItemID_
and GoodsID % 10 = 0
and ShowWeb = 1
and Cash = 0
and Gem = 0
and Crystal = 0
and Price <= @iGamePoint
and Price <> 0

if @bSetID is null
begin    select @iOK = -2    goto end_proc    end

declare
    @Item table
(
    No int not null identity(1, 1)
,   ItemID int not null
,   ItemUID int null
)
if @bSetID = 0
begin
    insert into @Item
    (
        ItemID
    )
    select
        @iItemID_
end
else if @bSetID = 1
begin
    insert into @Item
    (
        ItemID
    )
    select
        GoodsID
    from dbo.GoodsInfoList
    where
        SetGoodsNum = @iSetID
    and GoodsID % 10 = 0
    and IsSubSetGoods = 0
    and Kind in ( 0, 1, 2, 8, 9, 13 )
end

if @bItemType = 0
begin
    update a
    set
        a.ItemUID = b.ItemUID
    from @Item
    as a
    join
    (
        select
            ItemUID
        ,   ItemID
        from dbo.GoodsObjectList
        where
            OwnerLogin = @strLogin
    )
    as b
    on
        a.ItemID = b.ItemID
end
else if @bItemType = 1
begin
    update a
    set
        a.ItemUID = b.DurationItemUID
    from @Item
    as a
    join
    (
        select
            DurationItemUID
        ,   GoodsID
        from dbo.DurationItemObjectList
        where
            OwnerLogin = @strLogin
    )
    as b
    on
        a.ItemID = b.GoodsID
end


declare
    @iLoop tinyint
,   @iLoopMax tinyint
,   @sdtDateB smalldatetime
,   @iItemID int
,   @iItemUID int
select
    @iLoop = 1
,   @iLoopMax = max(No)
,   @sdtDateB = current_timestamp
from @Item

begin transaction

while @iLoop <= @iLoopMax
begin
    select
        @iItemID = ItemID
    ,   @iItemUID = ItemUID
    from @Item
    where
        No = @iLoop

    if @bItemType = 0
    begin
        if @iItemUID is not null
        and @iFactor <> -1
        begin
            update a
            with (updlock)
            set
                EndDate = dateadd(dd, @iFactor, EndDate)
            ,   Period = Period + @iFactor
            from dbo.GoodsObjectList
            as a
            with (updlock)
            where
                ItemUID = @iItemUID

            if @@error <> 0
            begin    select @iOK = -101    goto fail_tran    end
        end
        else
        begin
            insert into dbo.GoodsObjectList
            with (updlock)
            (
                OwnerLogin
            ,   BuyerLogin
            ,   ItemID
            ,   RegDate
            ,   StartDate
            ,   EndDate
            ,   Period
            )
            select
                @strLogin
            ,   N'__mc__20081218__01__'
            ,   @iItemID
            ,   @sdtDateB
            ,   @sdtDateB
            ,   dateadd(dd, @iFactor, @sdtDateB)
            ,   @iFactor

            if @@error <> 0
            begin    select @iOK = -102    goto fail_tran    end

            update a
            set
                ItemUID = @@identity
            from @Item
            as a
            where
                No = @iLoop
        end
    end
    else if @bItemType = 1
    begin
        if @iItemUID is not null
        begin
            update a
            with (updlock)
            set
                Duration = Duration + @iFactor
            from dbo.DurationItemObjectList
            as a
            with (updlock)
            where
                DurationItemUID = @iItemUID

            if @@error <> 0
            begin    select @iOK = -103    goto fail_tran    end
        end
        else
        begin
            insert into dbo.DurationItemObjectList
            with (updlock)
            (
                OwnerLogin
            ,   BuyerLogin
            ,   GoodsID
            ,   Duration
            ,   RegDate
            )
            select
                @strLogin
            ,   N'__mc__20081218__01__'
            ,   @iItemID
            ,   @iFactor
            ,   @sdtDateB

            if @@error <> 0
            begin    select @iOK = -104    goto fail_tran    end

            update a
            set
                ItemUID = @@identity
            from @Item
            as a
            where
                No = @iLoop
        end
    end

    select
        @iLoop = @iLoop + 1
end

update a
with (updlock)
set
    @iGamePoint = GamePoint = GamePoint - @iPrice
from dbo.Users
as a
with (updlock)
where
    Login = @strLogin

if @@error <> 0
begin    select @iOK = -105    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK
,   @iGamePoint
,   @iOK
,   @iOK
,   @sdtDateB
,   @sdtDateB
,   @sdtDateB
union all
select
    @bItemType
,   ItemID
,   ItemUID
,   @iFactor
,   @sdtDateB
,   @sdtDateB
,   dateadd(dd, @iFactor, @sdtDateB)
from @Item


