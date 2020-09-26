CREATE PROCEDURE [dbo].[RSG_recommend_system_r_update_ni]
	@iLoginUID_ [int],
	@strNick_ [nvarchar](24) = null,
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 4월 16일
사용 : 반갑다 친구야 시즌2 추천한 님의 추천받은 님 정보 갱신
*/
set nocount on

declare
    @sdtDate smalldatetime
select
    @sdtDate = getdate()
if not exists
(
    select *
    from dbo.RSGRecommendSystem
    with (nolock)
    where
        @sdtDate between StartDate and EndDate
)
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -2    goto end_proc    end

if not exists
(
    select *
    from dbo.RSGRecommendSystemR
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and Recommended = 0
    and Supplied = 0
)
begin    select @iOK = -3    goto end_proc    end

declare
    @iELoginUID int
select
    @iELoginUID = -1
select
    @iELoginUID = a.LoginUID
from dbo.Users as a
with (nolock)
join dbo.NickNames as b
with (nolock)
on a.Login = b.Login
where b.Nick = @strNick_

begin tran
    update a
    with (updlock)
    set
        a.ELoginUID = @iELoginUID
    ,   a.RecomDate = dateadd(mi, 1, @sdtDate)
    from dbo.RSGRecommendSystemR as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


