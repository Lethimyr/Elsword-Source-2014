CREATE PROCEDURE [dbo].[dup_main_package_delete]
	@iMainItemID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 6월 29일
1차 수정일 : 2008년 7월 8일
사용 : 메인 패키지 삭제
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
        if @@error <> 0
        begin    select @iOK = -11    goto fail_tran    end
    commit tran
end
else
begin
    begin tran
        delete a
        with (updlock)
        from dbo.CashPackageInfoList
        as a
        with (updlock)
        where
            ItemID = @iMainItemID_
        if @@error <> 0
        begin    select @iOK = -12    goto fail_tran    end

        delete a
        with (updlock)
        from dbo.CashPackageInfoDetailList
        as a
        with (updlock)
        where
            ItemID = @iMainItemID_
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


