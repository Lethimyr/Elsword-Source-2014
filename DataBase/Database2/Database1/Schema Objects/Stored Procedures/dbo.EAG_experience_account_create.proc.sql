CREATE PROCEDURE [dbo].[EAG_experience_account_create]
	@strLoginE_ [nvarchar](10),
	@strPasswdE_ [nvarchar](32),
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

declare
    @iOKTemp int
select
    @iOKTemp = 0

begin tran
    exec @iOKTemp = dbo.up_insert_new_user
        @strLoginE
    ,   @strPasswdE_
    ,   '1'
    ,   @bReturned = 1
    if @iOKTemp <> 0
    begin    select @iOK = -101    goto fail_tran    end

    insert into dbo.EAGExperienceAccount
    with (updlock)
    (
        Login
    )
    select
        @strLoginE_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

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
    ,   1 --생성
    ,   N''
    ,   @strPasswdE_
    ,   N''
    ,   -1
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


