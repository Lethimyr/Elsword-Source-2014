CREATE PROCEDURE [dbo].[RSG_recommend_system_e_check]
	@iLoginUID_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 4월 16일
사용 : 반갑다 친구야 시즌2 추천받은 님의 정보 출력
*/
set nocount on

declare
    @iCount int
select
    @iCount = 0

if not exists
(
    select *
    from dbo.RSGRecommendSystem
    with (nolock)
    where
        getdate() between StartDate and EndDate
)
begin    select @iOK = -1    goto end_proc    end

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
with (nolock)
where
    LoginUID = @iLoginUID_
if @strLogin is null
begin    select @iOK = -2    goto end_proc    end

declare
    @iCountTemp int
select
    @iCountTemp = RecomCount - SupCount
from dbo.RSGRecommendSystemE
with (nolock)
where
    LoginUID = @iLoginUID_
and RecomCount >= SupCount
if @iCountTemp is null
begin    select @iOK = -2    goto end_proc    end

select
    @iCount = @iCountTemp

if @iCount > 0
begin
    declare
        @strItemList nvarchar(4000)
    ,   @iOKTemp int
    select
        @strItemList
            = N'19360,' + convert(nvarchar(10), @iCount * 10) + N','
            + N'58630,' + convert(nvarchar(10), @iCount * 10) + N','
            + N'8880,'  + convert(nvarchar(10), @iCount * 10) + N','
            + N'76760,' + convert(nvarchar(10), @iCount * 1 ) + N','

    begin tran
        update a
        with (updlock)
        set
            a.SupCount = a.RecomCount
        from dbo.RSGRecommendSystemE as a
        with (updlock)
        where
            LoginUID = @iLoginUID_
        if @@error <> 0
        begin    select @iOK = -101    goto fail_tran    end

        exec @iOKTemp = dbo.up_insert_item
            @strLogin
        ,   @strItemList
        ,   @bReturn_ = 0
        if @iOKTemp <> 0
        begin    select @iOK = -102    goto fail_tran    end
    commit tran
end

goto end_proc

fail_tran:
rollback tran

end_proc:
declare
    @iSupCount int
,   @iSupType int
select
    @iSupCount = 0
,   @iSupType = 0
select
    @iSupCount = SupCount
,   @iSupType = 15
        - SupType
        - case
            when SupCount < 3  then 15
            when SupCount < 6  then 14
            when SupCount < 9  then 12
            when SupCount < 12 then 8
            else 0
        end
from dbo.RSGRecommendSystemE
with (nolock)
where
    LoginUID = @iLoginUID_

select
    @iCount
,   @iSupCount
,   @iSupType


