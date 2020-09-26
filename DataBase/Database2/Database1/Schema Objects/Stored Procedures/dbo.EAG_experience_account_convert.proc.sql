CREATE PROCEDURE [dbo].[EAG_experience_account_convert]
	@strLoginE_ [nvarchar](10),
	@strLoginF_ [nvarchar](20),
	@strPasswdF_ [nvarchar](32),
	@strSexF_ [char](1),
	@iOK [int] = 0 OUTPUT,
	@strData [nvarchar](50) = N'' OUTPUT
AS
set nocount on

--Status
--1 : 생성(create)   ; Data2(체험 Passwd)
--2 : 탈퇴(secede)   ; 
--3 : 전환(convert)  ; Data1(정식 Login), Data2(정식 Passwd), Data3(정식 Sex), Data4(정식 LoginUID)
--4 : 롤백(rollback) ; Data1(정식 Login), Data2(체험 Passwd), Data4(정식 LoginUID)
--5 : 전환롤백       ; 3 -> 5 로 상태 변경

/*
OK(우리값 / 넷마블요청값 / 설명)
       0 /     1 / 성공
      -1 / -3001 / 체험계정이 아님
      -2 / -3002 / 체험계정정보가 없음
      -3 / -9003 / 체험계정이 존재하며 접속중에 있음
      -4 / -9002 / 정식계정이 존재하며 접속중에 있음
-101이하 / -9999 / 트랜잭션 에러
*/

select
    @iOK = 1
,   @strData = N'-1레벨-1승-1패'

if not exists
(
    select *
    from dbo.EAGExperienceAccount
    with (nolock)
    where
        Login = @strLoginE_
)
begin    select @iOK = -3001    goto end_proc    end

declare
    @strLoginE nvarchar(20)
select
    @strLoginE = N'kkurrung1_' + @strLoginE_

declare
    @iLoginUIDE int
,   @sdtLastLoginE smalldatetime
,   @bConnectingE bit
select
    @iLoginUIDE = LoginUID
,   @sdtLastLoginE = LastLogin
,   @bConnectingE = Connecting
from dbo.Users
with (nolock)
where
    Login = @strLoginE

if @iLoginUIDE is null
begin    select @iOK = -3002    goto end_proc    end

if exists
(
    select *
    from dbo.LSGLoginServerConnectStatus
    with (nolock)
    where
        LoginUID = @iLoginUIDE
    and Connecting = 1
)
or
(
    dateadd(mi, -10, getdate()) <= @sdtLastLoginE
and @bConnectingE = 1
)
begin    select @iOK = -9003    goto end_proc    end

declare
    @iLoginUIDF int
,   @sdtLastLoginF smalldatetime
,   @bConnectingF bit
,   @strLoginF nvarchar(20)
select
    @iLoginUIDF = LoginUID
,   @sdtLastLoginF = LastLogin
,   @bConnectingF = Connecting
,   @strLoginF = N'kkurrung2_' + convert(nvarchar(10), LoginUID)
from dbo.Users
with (nolock)
where
    Login = @strLoginF_

if @iLoginUIDF is not null
and
(
    exists
    (
        select *
        from dbo.LSGLoginServerConnectStatus
        with (nolock)
        where
            LoginUID = @iLoginUIDF
        and Connecting = 1
    )
or  dateadd(mi, -10, getdate()) <= @sdtLastLoginF
and @bConnectingF = 1
)
begin    select @iOK = -9002    goto end_proc    end

begin tran
    if @iLoginUIDF is not null
    begin
        update a
        with (updlock)
        set
            a.Login = @strLoginF
        from dbo.Users
        as a
        with (updlock)
        where
            Login = @strLoginF_
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -9999    goto fail_tran    end

        update a
        with (updlock)
        set
            a.OwnerLogin = @strLoginF
        from dbo.GoodsObjectList
        as a
        with (updlock)
        where
            OwnerLogin = @strLoginF_
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -9999    goto fail_tran    end

        update a
        with (updlock)
        set
            a.OwnerLogin = @strLoginF
        from dbo.DurationItemObjectList
        as a
        with (updlock)
        where
            OwnerLogin = @strLoginF_
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -9999    goto fail_tran    end

        update a
        with (updlock)
        set
            a.Login = @strLoginF
        from dbo.Agreement
        as a
        with (updlock)
        where
            Login = @strLoginF_
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -9999    goto fail_tran    end

        insert into dbo.UAGUserAuth
        with (updlock)
        (
            LoginUID
        ,   AuthLevel
        )
        select
            @iLoginUIDF
        ,   -3
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -9999    goto fail_tran    end
    end

    update a
    with (updlock)
    set
        a.Login = @strLoginF_
    ,   a.passwd = @strPasswdF_
    from dbo.Users
    as a
    with (updlock)
    where
        Login = @strLoginE
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -9999    goto fail_tran    end

    update a
    with (updlock)
    set
        a.OwnerLogin = @strLoginF_
    from dbo.GoodsObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLoginE
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -9999    goto fail_tran    end

    update a
    with (updlock)
    set
        a.OwnerLogin = @strLoginF_
    from dbo.DurationItemObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLoginE
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -9999    goto fail_tran    end

    update a
    with (updlock)
    set
        a.Login = @strLoginF_
    from dbo.Agreement
    as a
    with (updlock)
    where
        Login = @strLoginE
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -9999    goto fail_tran    end

    delete a
    with (updlock)
    from dbo.EAGExperienceAccount
    as a
    with (updlock)
    where
        Login = @strLoginE_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -9999    goto fail_tran    end

    insert into dbo.EALExperienceAccount
    with (updlock)
    (
        Login
    ,   RegDateA
    ,   Status
    ,   Data1
    ,   Data2
    ,   Data3
    ,   Data4
    )
    select
        @strLoginE_
    ,   getdate()
    ,   3 --전환
    ,   @strLoginF_
    ,   @strPasswdF_
    ,   @strSexF_
    ,   case when @iLoginUIDF is null then -1 else @iLoginUIDF end
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -9999    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK
return
    @iOK


