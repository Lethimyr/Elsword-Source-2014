CREATE PROCEDURE [dbo].[dup_package_insert]
	@iMainItemID_ [int],
	@iSubItemID_ [int],
	@iSubFactor_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 6월 29일
1차 수정일 : 2008년 7월 8일
사용 : 서브 패키지 삭제
*/
set nocount on

if not exists
(
    select *
    from dbo.GoodsInfoList
    with (nolock)
    where
        GoodsID = @iMainItemID_
)
begin    select @iOK = -1    goto end_proc    end

declare
    @iItemType int
select
    @iItemType = ItemType
from dbo.GoodsInfoList
    with (nolock)
where
    GoodsID = @iSubItemID_

if @iItemType is null
    begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.GlobalVariables
    with (nolock)
    where
        No = 7
    and Variable1 in
    (
        1 --국내
    ,   8 --미국
    )
)
begin
    begin tran
        insert into dbo.PackageInfoList
        with (updlock)
        (
            ItemID
        ,   PackageItemID
        ,   ItemType
        ,   Factor
        ,   SupplyCount
        ,   SupplyDate
        )
        select
            @iMainItemID_
        ,   @iSubItemID_
        ,   @iItemType
        ,   @iSubFactor_
        ,   -2147483648
        ,   0
        if @@error <> 0
        begin    select @iOK = -11    goto fail_tran    end
    commit tran
end
else
begin
    declare
        @bInsertMain bit
    select
        @bInsertMain = 0
    if not exists
    (
        select *
        from dbo.CashPackageInfoDetailList
        with (nolock)
        where
            ItemID = @iMainItemID_
    )
    begin    select @bInsertMain = 1    end

    begin tran
        if @bInsertMain = 1
        begin
            insert into dbo.CashPackageInfoList
            with (updlock)
            (
                ItemID
            ,   SupplyCount
            ,   SupplyDate
            )
            select
                @iMainItemID_
            ,   -1
            ,   0
            if @@error <> 0
            begin    select @iOK = -12    goto fail_tran    end
        end

        insert into dbo.CashPackageInfoDetailList
        with (updlock)
        (
            ItemID
        ,   ItemType
        ,   SubItemID
        ,   Factor
        ,   SupplyCount
        ,   SupplyDate
        )
        select
            @iMainItemID_
        ,   @iItemType
        ,   @iSubItemID_
        ,   @iSubFactor_
        ,   -1
        ,   0
        if @@error <> 0
        begin    select @iOK = -13    goto fail_tran    end
    commit tran
end

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK as OK


