CREATE PROCEDURE [dbo].[RSG_recommend_system_r_check]
	@iLoginUID_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 4월 16일
사용 : 반갑다 친구야 시즌2 조건 체크
*/
set nocount on

declare
    @sdtDate smalldatetime
,   @sdtLimitDate smalldatetime
select
    @sdtDate = getdate()
select
    @sdtLimitDate = LimitDate
from dbo.RSGRecommendSystem
with (nolock)
where
    @sdtDate between StartDate and EndDate
if @sdtLimitDate is null
begin    select @iOK = -1    goto end_proc    end

declare
    @strLogin nvarchar(20)
,   @iUserType tinyint
select
    @strLogin = Login
,   @iUserType =
    case
        when FirstLogin = LastLogin then 1
        when LastLogin < @sdtLimitDate then 2
    end
from dbo.Users
with (nolock)
where
    LoginUID = @iLoginUID_
if @strLogin is null
begin    select @iOK = -2    goto end_proc    end

declare
    @iCheckData int
,   @iELoginUID int
,   @bRecommended bit
,   @bSupplied bit

select
    @iUserType = UserType
,   @iCheckData = CheckData
,   @iELoginUID = ELoginUID
,   @bRecommended = Recommended
,   @bSupplied = Supplied
from dbo.RSGRecommendSystemR
with (nolock)
where LoginUID = @iLoginUID_

if @iUserType is not null
and @iCheckData is null
begin
    select
        @iCheckData = 0
    ,   @iELoginUID = -1
    ,   @bRecommended = 0
    ,   @bSupplied = 0
    
    begin tran
        insert into dbo.RSGRecommendSystemR
        with (updlock)
        (
            LoginUID
        ,   UserType
        ,   CheckData
        ,   ELoginUID
        ,   RegDate
        ,   RecomDate
        ,   SupDate
        )
        select
            @iLoginUID_
        ,   @iUserType
        ,   @iCheckData
        ,   @iELoginUID
        ,   @sdtDate
        ,   @sdtDate
        ,   @sdtDate
        if @@error <> 0
        begin    select @iOK = -101    goto fail_tran    end
    commit tran
end

if @iUserType is null
begin    goto end_proc    end
else if @bRecommended = 0
and @bSupplied = 0
begin    select @iOK = 1    goto end_proc    end
else if @bRecommended = 1
and @bSupplied = 0
begin
    select
        @iOK = 2

    if @iCheckData >=
        case @iUserType
            when 1 then 15
            when 2 then 180
        end
    begin
        select
            @iOK = 3

        begin tran
            update a
            with (updlock)
            set SupDate = dateadd(mi, 2, @sdtDate)
            from dbo.RSGRecommendSystemR as a
            with (updlock)
            where LoginUID = @iLoginUID_
            if @@error <> 0
            begin    select @iOK = -102    goto fail_tran    end

            if @iELoginUID <> -1
            begin
                if exists
                (
                    select *
                    from dbo.RSGRecommendSystemE
                    with (nolock)
                    where LoginUID = @iELoginUID
                )
                begin
                    update a
                    with (updlock)
                    set a.RecomCount = a.RecomCount + case a.RecomCount when 255 then 0 else 1 end
                    from dbo.RSGRecommendSystemE as a
                    with (updlock)
                    where LoginUID = @iELoginUID
                    if @@error <> 0
                    begin    select @iOK = -103    goto fail_tran    end
                end
                else
                begin
                    insert into dbo.RSGRecommendSystemE
                    with (updlock)
                    (
                        LoginUID
                    ,   RecomCount
                    ,   SupCount
                    ,   SupType
                    )
                    select
                        @iELoginUID
                    ,   1
                    ,   0
                    ,   0
                    if @@error <> 0
                    begin    select @iOK = -104    goto fail_tran    end
                end
            end

            declare
                @iOKTemp int
            ,   @strItemList nvarchar(4000)
            select
                @strItemList =
                    case @iUserType
                        when 1 then N'19360,30,42410,3,38880,7,38860,1,'
                        when 2 then N'58720,10,58750,10,76760,1,76740,1,76720,1,'
                    end
            exec @iOKTemp = dbo.up_insert_item
                @strLogin
            ,   @strItemList
            ,   @bReturn_ = 0
            if @iOKTemp <> 0
            begin    select @iOK = -105    goto fail_tran    end

            insert into dbo.CMGConnectingMessageUser
            with (updlock)
            (
                LoginUID
            ,   MessageID
            ,   LimitDate
            ,   Readed
            )
            select
                @iLoginUID_
            ,   2008042801
            ,   dateadd(dd, 7, convert(nvarchar(8), @sdtDate, 112))
            ,   0
            if @@error <> 0
            begin    select @iOK = -105    goto fail_tran    end
        commit tran
    end
end
else if @bRecommended = 1
and @bSupplied = 1
begin    select @iOK = 4    goto end_proc    end
else
begin    select @iOK = -3    goto end_proc    end

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


