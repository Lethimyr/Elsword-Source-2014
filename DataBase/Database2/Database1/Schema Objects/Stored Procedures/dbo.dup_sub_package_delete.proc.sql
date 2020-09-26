CREATE PROCEDURE [dbo].[dup_sub_package_delete]
	@iMainItemID_ [int],
	@iSubItemID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 6월 29일
1차 수정일 : 2008년 7월 8일
사용 : 서브 패키지 삭제
*/
set nocount on;

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
        delete a
        with (updlock)
        from dbo.PackageInfoList
        as a
        with (updlock)
        where
            ItemID = @iMainItemID_
        and PackageItemID = @iSubItemID_
        if @@error <> 0
        begin    select @iOK = -11    goto fail_tran    end
    commit tran
end
else
begin
    declare
        @bDeleteMain bit
    select
        @bDeleteMain = 0
    if 1 =
    (
        select
            count(*)
        from dbo.CashPackageInfoDetailList
        with (nolock)
        where
            ItemID = @iMainItemID_
    )
    begin    select @bDeleteMain = 1    end

    begin tran
        if @bDeleteMain = 1
        begin
            delete a
            with (updlock)
            from dbo.CashPackageInfoList
            as a
            with (updlock)
            where
                ItemID = @iMainItemID_
            if @@error <> 0
            begin    select @iOK = -12    goto fail_tran    end
        end

        delete a
        with (updlock)
        from dbo.CashPackageInfoDetailList
        as a
        with (updlock)
        where
            ItemID = @iMainItemID_
        and SubItemID = @iSubItemID_
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


