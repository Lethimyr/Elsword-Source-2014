CREATE PROCEDURE [dbo].[EAG_experience_account_rollback]
	@strLoginE_ [nvarchar](10),
	@strPasswdE_ [nvarchar](32),
	@strLoginF_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on

--Status
--1 : 생성(create)   ; Data2(체험 Passwd)
--2 : 탈퇴(secede)   ; 
--3 : 전환(convert)  ; Data1(정식 Login), Data2(정식 Passwd), Data3(정식 Sex), Data4(정식 LoginUID)
--4 : 롤백(rollback) ; Data1(정식 Login), Data2(체험 Passwd), Data4(정식 LoginUID)
--5 : 전환롤백       ; 3 -> 5 로 상태 변경

if exists
(
    select *
    from dbo.EAGExperienceAccount
    with (nolock)
    where
        Login = @strLoginE_
)
begin    select @iOK = -1    goto end_proc    end

declare
    @strLoginE nvarchar(20)
select
    @strLoginE = N'kkurrung1_' + @strLoginE_

if exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        Login = @strLoginE
)
begin    select @iOK = -2    goto end_proc    end

if not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        Login = @strLoginF_
)
begin    select @iOK = -3    goto end_proc    end

declare
    @iLoginUIDF int
,   @strLoginF nvarchar(20)
select
    @iLoginUIDF = Data4
,   @strLoginF = N'kkurrung2_' + convert(nvarchar(10), Data4)
from dbo.EALExperienceAccount
with (nolock)
where
    Login = @strLoginE_
and Status = 3
and Data1 = @strLoginF_

if @iLoginUIDF is null
begin    select @iOK = -4    goto end_proc    end

if @iLoginUIDF <> -1
and not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        Login = @strLoginF
)
begin    select @iOK = -5    goto end_proc    end

begin tran
    update a
    with (updlock)
    set
        a.Login = @strLoginE
    ,   a.passwd = @strPasswdE_
    from dbo.Users
    as a
    with (updlock)
    where
        Login = @strLoginF_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    update a
    with (updlock)
    set
        a.OwnerLogin = @strLoginE
    from dbo.GoodsObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLoginF_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    update a
    with (updlock)
    set
        a.OwnerLogin = @strLoginE
    from dbo.DurationItemObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLoginF_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end

    update a
    with (updlock)
    set
        a.Login = @strLoginE
    from dbo.Agreement
    as a
    with (updlock)
    where
        Login = @strLoginF_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -104    goto fail_tran    end

    if @iLoginUIDF <> -1
    begin
        delete a
        with (updlock)
        from dbo.UAGUserAuth
        as a
        with (updlock)
        where
            LoginUID = @iLoginUIDF
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -105    goto fail_tran    end

        update a
        with (updlock)
        set
            a.Login = @strLoginF_
        from dbo.Users
        as a
        with (updlock)
        where
            Login = @strLoginF
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -106    goto fail_tran    end

        update a
        with (updlock)
        set
            a.OwnerLogin = @strLoginF_
        from dbo.GoodsObjectList
        as a
        with (updlock)
        where
            OwnerLogin = @strLoginF
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -107    goto fail_tran    end

        update a
        with (updlock)
        set
            a.OwnerLogin = @strLoginF_
        from dbo.DurationItemObjectList
        as a
        with (updlock)
        where
            OwnerLogin = @strLoginF
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -108    goto fail_tran    end

        update a
        with (updlock)
        set
            a.Login = @strLoginF_
        from dbo.Agreement
        as a
        with (updlock)
        where
            Login = @strLoginF
        option (maxdop 1)
        if @@error <> 0
        begin    select @iOK = -109    goto fail_tran    end
    end

    insert into dbo.EAGExperienceAccount
    with (updlock)
    (
        Login
    )
    select
        @strLoginE_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -110    goto fail_tran    end

    update a
    with (updlock)
    set
        a.Status = 5
    from dbo.EALExperienceAccount
    as a
    with (updlock)
    where
        Login = @strLoginE_
    and Status = 3
    and Data1 = @strLoginF_    
    if @@error <> 0
    begin    select @iOK = -111    goto fail_tran    end

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
    ,   4 --롤백
    ,   @strLoginF_
    ,   @strPasswdE_
    ,   N''
    ,   @iLoginUIDF
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -112    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


