CREATE PROCEDURE [dbo].[PIG_package_item_select]
AS
/*microcat
작성일 : 2008년 5월 26일
사용 : 패키지 리스트 출력
*/
set nocount on

if exists
(
    select *
    from dbo.GlobalVariables
    with (nolock)
    where
        No = 7
    and Variable1 = 1
)
begin
    select
        ItemID
    ,   PackageItemID
    ,   Factor
    from dbo.PackageInfoList
    with (nolock)
    option (maxdop 1)
end
else
begin
    select
        ItemID
    ,   SubItemID
    ,   Factor
    from dbo.CashPackageInfoDetailList
    with (nolock)
    option (maxdop 1)
end


