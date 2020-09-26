CREATE PROCEDURE [dbo].[UAD_user_auth_update]
	@strLogin_ [nvarchar](20),
	@iAction_ [int],
	@strIPs_ [nvarchar](15) = N'0.0.0.0',
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 4월 7일
사용 : dbot에서 유저 인증을 변경합니다.
*/
set nocount on

declare
    @iAction int
,   @dtDate datetime
,   @iLoginUID int
,   @iAuthLevel1 int
,   @sdtLimitDate1 smalldatetime
,   @iStatus1 int
,   @iStatus2 int
select
    @iAction = - @iAction_ / 10
,   @iStatus2 = @iAction_ % 10
,   @dtDate = getdate()

select
    @iLoginUID = LoginUID
from dbo.Users
with (nolock)
where Login = @strLogin_
if @iLoginUID is null
begin    select @iOK = -1    goto end_proc    end

select
    @iAuthLevel1 = AuthLevel
from dbo.UAGUserAuth
with (nolock)
where LoginUID = @iLoginUID

select
    @sdtLimitDate1 = LimitDate
from dbo.UAGUserAuthDate
with (nolock)
where LoginUID = @iLoginUID

select
    @iStatus1 = Status
from dbo.UAGUserAuthStatus
with (nolock)
where LoginUID = @iLoginUID

begin tran
    if @iAction = 0
    begin
        delete a
        with (updlock)
        from dbo.UAGUserAuth as a
        with (updlock)
        where LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -101    goto fail_tran    end
    end
    else if @iAuthLevel1 is null
    begin
        insert into dbo.UAGUserAuth
        with (updlock)
        (
            LoginUID
        ,   AuthLevel
        )
        select
            @iLoginUID
        ,   case @iAction
            when -4 then -3
            else @iAction
            end
        if @@error <> 0
        begin    select @iOK = -102    goto fail_tran    end
    end
    else if @iAuthLevel1 is not null
    begin
        update a
        with (updlock)
        set
            AuthLevel =
            case @iAction
            when -4 then -3
            else @iAction
            end
            + case when @iAuthLevel1 = -1 and @iAction = -1 then -1 else 0 end
        from dbo.UAGUserAuth as a
        with (updlock)
        where LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -103    goto fail_tran    end
    end

    if @iAction not between -3 and -1
    begin
        delete a
        with (updlock)
        from dbo.UAGUserAuthDate as a
        with (updlock)
        where LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -104    goto fail_tran    end
    end
    else if @sdtLimitDate1 is null
    begin
        insert into dbo.UAGUserAuthDate
        with (updlock)
        (
            LoginUID
        ,   LimitDate
        )
        select
            @iLoginUID
        ,   dateadd
            (
                dd
            ,   case @iAction
                when -1 then 7
                when -2 then 14
                when -3 then 30
                end
            ,   @dtDate
            )
        if @@error <> 0
        begin    select @iOK = -105    goto fail_tran    end
    end
    else if @sdtLimitDate1 is not null
    begin
        update a
        with (updlock)
        set
            LimitDate =
            dateadd
            (
                dd
            ,   case @iAction
                when -1 then 7
                when -2 then 14
                when -3 then 30
                end
                + case when @iAuthLevel1 = -1 and @iAction = -1 then 13 else 0 end
            ,   @dtDate
            )
        from dbo.UAGUserAuthDate as a
        with (updlock)
        where LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -106    goto fail_tran    end
    end

    if @iAction not between -2 and -1
    begin
        delete a
        with (updlock)
        from dbo.UAGUserAuthStatus as a
        with (updlock)
        where LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -107    goto fail_tran    end
    end
    else if @iStatus1 is null
    begin
        insert into dbo.UAGUserAuthStatus
        with (updlock)
        (
            LoginUID
        ,   Status
        )
        select
            @iLoginUID
        ,   @iStatus2
        if @@error <> 0
        begin    select @iOK = -108    goto fail_tran    end
    end
    else if @iStatus1 is not null
    begin
        update a
        with (updlock)
        set
            Status = @iStatus2
        from dbo.UAGUserAuthStatus as a
        with (updlock)
        where LoginUID = @iLoginUID
        if @@error <> 0
        begin    select @iOK = -109    goto fail_tran    end
    end

    insert into dbo.UALUserAuth
    with (updlock)
    (
        LoginUID
    ,   RegDate
    ,   AuthLevel1
    ,   LimitDate1
    ,   Status1
    ,   AuthLevel2
    ,   LimitDate2
    ,   Status2
    ,   IPn
    )
    select
        @iLoginUID
    ,   @dtDate
    ,   @iAuthLevel1
    ,   @sdtLimitDate1
    ,   @iStatus1
    ,   case @iAction
        when -4 then -3
        else @iAction
        end
        + case when @iAuthLevel1 = -1 and @iAction = -1 then -1 else 0 end
    ,   dateadd
        (
            dd
        ,   case @iAction
            when -1 then 7
            when -2 then 14
            when -3 then 30
            end
            + case when @iAuthLevel1 = -1 and @iAction = -1 then 13 else 0 end
        ,   @dtDate
        )
    ,   @iStatus2
    ,   dbo.IPn_IPs( @strIPs_ )
    if @@error <> 0
    begin    select @iOK = -110    goto fail_tran    end

commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK as OK


