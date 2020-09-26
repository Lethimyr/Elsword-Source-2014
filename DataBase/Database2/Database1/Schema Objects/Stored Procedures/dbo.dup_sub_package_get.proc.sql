CREATE PROCEDURE [dbo].[dup_sub_package_get]
AS
/*microcat
작성일 : 2007년 6월 29일
1차 수정일 : 2008년 7월 8일
사용 : 서브 패키지 리스트 출력
*/
set nocount on

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
    select
        a.ItemID as MainItemID
    ,   a.ItemType as SubItemType
    ,   a.PackageItemID as SubItemID
    ,   isnull(replace(b.GoodsName, '%', 'p'), N'Not Defined') as SubItemName
    ,   a.Factor as SubFactor
    from dbo.PackageInfoList
    as a
    with (nolock)
    left outer join dbo.GoodsInfoList
    as b
    with (nolock)
    on
        a.PackageItemID = b.GoodsID
    order by
        a.ItemID
    ,   a.PackageItemID
    option (maxdop 1)
end
else
begin
    select
        a.ItemID as MainItemID
    ,   a.ItemType as SubItemType
    ,   a.SubItemID as SubItemID
    ,   isnull(replace(b.GoodsName, '%', 'p'), N'Not Defined') as SubItemName
    ,   a.Factor as SubFactor
    from dbo.CashPackageInfoDetailList
    as a
    with (nolock)
    left outer join dbo.GoodsInfoList
    as b
    with (nolock)
    on
        a.SubItemID = b.GoodsID
    order by
        a.ItemID
    ,   a.SubItemID
    option (maxdop 1)
end


