CREATE PROCEDURE [dbo].[RSG_recommend_system_r_update_cd]
	@iLoginUID_ [int],
	@iCheckData_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 4월 16일
사용 : 반갑다 친구야 시즌2 추천한 님의 정보 갱신
*/
set nocount on

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
where LoginUID = @iLoginUID_
if @strLogin is null
begin    select @iOK = -2    goto end_proc    end

declare
    @iUserType tinyint
select
    @iUserType = UserType
from dbo.RSGRecommendSystemR
with (nolock)
where
    LoginUID = @iLoginUID_
and Recommended = 1
and Supplied = 0
if @iUserType is null
begin    select @iOK = -3    goto end_proc    end

if @iUserType = 1
begin
    select
        @iCheckData_ = max(Level)
    from dbo.Characters
    with (nolock)
    where Login = @strLogin
    group by Login
end

if @iCheckData_ < 0
begin
    select
        @iCheckData_ = 0
end

begin tran
    update a
    with (updlock)
    set a.CheckData = case @iUserType when 1 then 0 else a.CheckData end + @iCheckData_
    from dbo.RSGRecommendSystemR as a
    with (updlock)
    where LoginUID = @iLoginUID_
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


