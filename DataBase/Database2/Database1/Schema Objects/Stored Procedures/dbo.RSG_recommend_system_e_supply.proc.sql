CREATE PROCEDURE [dbo].[RSG_recommend_system_e_supply]
	@iLoginUID_ [int],
	@iSupType_ [tinyint],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 4월 16일
사용 : 반갑다 친구야 시즌2 추천받은 님의 아이템 지급
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

if @iSupType_ not in (1, 2, 4, 8)
begin    select @iOK = -3    goto end_proc    end

if not exists
(
    select *
    from dbo.RSGRecommendSystemE
    with (nolock)
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -4    goto end_proc    end

if exists
(
    select *
    from dbo.RSGRecommendSystemE
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and SupType & @iSupType_ = @iSupType_
)
begin    select @iOK = -5    goto end_proc    end

declare
    @strItemList nvarchar(4000)
select
    @strItemList =
        case @iSupType_
            when 1 then N'59780,1,81600,1,'
            when 2 then N'59780,1,58700,1,81600,1,'
            when 4 then N'59780,1,42410,5,58700,1,58630,10,81600,1,'
            when 8 then N'79060,-1,38970,1,59780,1,42410,5,58700,1,81600,1,'
        end

begin tran
    update a
    with (updlock)
    set a.SupType = a.SupType + @iSupType_
    from dbo.RSGRecommendSystemE as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    exec dbo.up_insert_item
        @strLogin
    ,   @strItemList
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
if @iOK <> 0
begin
    declare @LogList table
    (
        [No] int Identity(1, 1) primary key not null,
        Login nvarchar(20) not null,
        ItemType int not null,
        ItemID int not null,
        Factor int not null,
        ItemUID int not null,
        RegDate smalldatetime not null,
        StartDate smalldatetime not null,
        EndDate smalldatetime not null
    )

    select
        @iOK
    ,   [Login]
    ,   ItemType
    ,   ItemID
    ,   Factor
    ,   ItemUID
    ,   RegDate
    ,   StartDate
    ,   EndDate
    from @LogList
end


