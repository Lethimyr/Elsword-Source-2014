CREATE PROCEDURE [dbo].[IWGA_item_waitingitem_merge]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081220. microcat.

L"{ call dbo.IWGA_item_waitingitem_merge ( %d ) }"

{ call dbo.IWGA_item_waitingitem_merge ( @1 ) }
@1 ; LoginUID int

1 return ( @1 )
@1 ; OK int
    0        ; 아이템을 받아야 됨
    -1       ; 유저 정보가 없음
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
    @Item table
(
    ItemID int not null
,   Factor int not null
,   ExhaustionType tinyint not null
,   PropertyType tinyint not null
)
insert into @Item
(
    ItemID
,   Factor
,   ExhaustionType
,   PropertyType
)
select
    a.ItemID
,   a.Factor
,   a.ExhaustionType
,   a.PropertyType
from dbo.WIItem
as a
join
(
    select
        IssueUID
    from dbo.WIUser
    where
        LoginUID = @iLoginUID_
    and Supplied = 0
)
as b
on
    a.IssueUID = b.IssueUID

declare
    @PermanenceItem table
(
    ItemID int not null
,   Factor int not null
)
insert into @PermanenceItem
(
    ItemID
,   Factor
)
select
    ItemID
,   Factor
from @Item
where
    ExhaustionType = 0 --here
and PropertyType = 0
union all
select
    a.GoodsID
,   c.Factor
from
(
    select
        SetGoodsNum
    ,   GoodsID
    from dbo.GoodsInfoList
    where
        IsSubSetGoods = 0
    and GoodsID % 10 = 0
    and Kind in (0, 1, 2, 8, 9, 13)
)
as a
join dbo.GoodsInfoList
as b
on
    a.SetGoodsNum = b.SetGoodsNum
join
(
    select
        ItemID
    ,   Factor
    from @Item
    where
        ExhaustionType = 0 --here
    and PropertyType = 1
)
as c
on
    b.GoodsID = c.ItemID
union all
select
    a.SubItemID
,   a.Factor
from
(
    select
        ItemID
    ,   SubItemID
    ,   Factor
    from dbo.CashPackageInfoDetailList
    where
        ItemType = 0 --here
    and Factor = -1 --here
    union all
    select
        ItemID
    ,   PackageItemID
    ,   Factor
    from dbo.PackageInfoList
    where
        ItemType = 0 --here
    and Factor = -1 --here
)
as a
join
(
    select
        ItemID
    from @Item
    where
        ExhaustionType = 0
    and PropertyType = 2
)
as b
on
    a.ItemID = b.ItemID

declare
    @CountItem table
(
    ItemID int not null
,   Factor int not null
)
insert into @CountItem
(
    ItemID
,   Factor
)
select
    ItemID
,   sum(Factor)
from
(
    select
        ItemID
    ,   Factor
    from @Item
    where
        ExhaustionType = 1 --here
    and PropertyType = 0
    union all
    select
        a.GoodsID
    ,   c.Factor
    from
    (
        select
            SetGoodsNum
        ,   GoodsID
        from dbo.GoodsInfoList
        where
            IsSubSetGoods = 0
        and GoodsID % 10 = 0
        and Kind in (0, 1, 2, 8, 9, 13)
    )
    as a
    join dbo.GoodsInfoList
    as b
    on
        a.SetGoodsNum = b.SetGoodsNum
    join
    (
        select
            ItemID
        ,   Factor
        from @Item
        where
            ExhaustionType = 1 --here
        and PropertyType = 1
    )
    as c
    on
        b.GoodsID = c.ItemID
    union all
    select
        a.SubItemID
    ,   a.Factor
    from
    (
        select
            ItemID
        ,   SubItemID
        ,   Factor
        from dbo.CashPackageInfoDetailList
        where
            ItemType = 1 --here
        and Factor = Factor --here
        union all
        select
            ItemID
        ,   PackageItemID
        ,   Factor
        from dbo.PackageInfoList
        where
            ItemType = 1 --here
        and Factor = Factor --here
    )
    as a
    join
    (
        select
            ItemID
        from @Item
        where
            ExhaustionType = 0
        and PropertyType = 2
    )
    as b
    on
        a.ItemID = b.ItemID
)
as x
group by
    ItemID

declare
    @PeriodItem table
(
    ItemID int not null
,   Factor int not null
)
insert into @PeriodItem
(
    ItemID
,   Factor
)
select
    ItemID
,   sum(Factor)
from
(
    select
        ItemID
    ,   Factor
    from @Item
    where
        ExhaustionType = 2 --here
    and PropertyType = 0
    union all
    select
        a.GoodsID
    ,   c.Factor
    from
    (
        select
            SetGoodsNum
        ,   GoodsID
        from dbo.GoodsInfoList
        where
            IsSubSetGoods = 0
        and GoodsID % 10 = 0
        and Kind in (0, 1, 2, 8, 9, 13)
    )
    as a
    join dbo.GoodsInfoList
    as b
    on
        a.SetGoodsNum = b.SetGoodsNum
    join
    (
        select
            ItemID
        ,   Factor
        from @Item
        where
            ExhaustionType = 2 --here
        and PropertyType = 1
    )
    as c
    on
        b.GoodsID = c.ItemID
    union all
    select
        a.SubItemID
    ,   a.Factor
    from
    (
        select
            ItemID
        ,   SubItemID
        ,   Factor
        from dbo.CashPackageInfoDetailList
        where
            ItemType = 0 --here
        and Factor > 0 --here
        union all
        select
            ItemID
        ,   PackageItemID
        ,   Factor
        from dbo.PackageInfoList
        where
            ItemType = 0 --here
        and Factor > 0 --here
    )
    as a
    join
    (
        select
            ItemID
        from @Item
        where
            ExhaustionType = 0
        and PropertyType = 2
    )
    as b
    on
        a.ItemID = b.ItemID
)
as x
group by
    ItemID

declare
    @dtDateA datetime
,   @strBuyerLogin nvarchar(20)
select
    @dtDateA = current_timestamp
,   @strBuyerLogin = N'__WaitingItem__'

begin transaction

update a
with (updlock)
set
    a.BuyerLogin = @strBuyerLogin
,   a.StartDate =
    case
        when a.StartDate < a.RegDate then @dtDateA
        else a.StartDate
    end
,   a.EndDate = dateadd(dd, a.Period + b.Factor, a.StartDate)
,   a.Period = a.Period + b.Factor
from dbo.GoodsObjectList
as a
with (updlock)
join @PeriodItem
as b
on
    a.ItemID = b.ItemID
where
    a.OwnerLogin = @strLogin
and a.Period > 0

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

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
,   @strBuyerLogin
,   ItemID
,   @dtDateA
,   @dtDateA
,   dateadd(dd, Factor, @dtDateA)
,   Factor
from @PermanenceItem
union all
select
    @strLogin
,   @strBuyerLogin
,   a.ItemID
,   @dtDateA
,   @dtDateA
,   dateadd(dd, Factor, @dtDateA)
,   a.Factor
from @PeriodItem
as a
left outer join
(
    select
        ItemID
    from dbo.GoodsObjectList
    where
        OwnerLogin = @strLogin
    and Period > 0
)
as b
on
    a.ItemID = b.ItemID
where
    b.ItemID is null

if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

update a
with (updlock)
set
    a.BuyerLogin = @strBuyerLogin
,   a.Duration = a.Duration + b.Factor
from dbo.DurationItemObjectList
as a
with (updlock)
join @CountItem
as b
on
    a.GoodsID = b.ItemID
where
    a.OwnerLogin = @strLogin

if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

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
,   @strBuyerLogin
,   a.ItemID
,   a.Factor
,   @dtDateA
from @CountItem
as a
left outer join
(
    select
        GoodsID
    from dbo.DurationItemObjectList
    where
        OwnerLogin = @strLogin
)
as b
on
    a.ItemID = b.GoodsID
where
    b.GoodsID is null

if @@error <> 0
begin    select @iOK = -104    goto fail_tran    end

update a
with (updlock)
set
    SupDate = dateadd(mi, 1, @dtDateA)
from dbo.WIUser
as a
with (updlock)
where
    LoginUID = @iLoginUID_
and Supplied = 0

if @@error <> 0
begin    select @iOK = -105    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


