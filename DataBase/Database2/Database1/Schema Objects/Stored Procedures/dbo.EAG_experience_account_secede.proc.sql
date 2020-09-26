CREATE PROCEDURE [dbo].[EAG_experience_account_secede]
	@strLoginE_ [nvarchar](10),
	@iOK [int] = 0 OUTPUT
AS
set nocount on

--Status
--1 : 생성(create)   ; Data2(체험 Passwd)
--2 : 탈퇴(secede)   ; 
--3 : 전환(convert)  ; Data1(정식 Login), Data2(정식 Passwd), Data3(정식 Sex), Data4(정식 LoginUID)
--4 : 롤백(rollback) ; Data1(정식 Login), Data2(체험 Passwd), Data4(정식 LoginUID)
--5 : 전환롤백       ; 3 -> 5 로 상태 변경

select
    @iOK = 0

if not exists
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
,   @bDeleteGameAccount bit
select
    @strLoginE = N'kkurrung1_' + @strLoginE_
,   @bDeleteGameAccount = 0

if exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        Login = @strLoginE
)
begin
    select @bDeleteGameAccount = 1
end

declare
    @iOKTemp int
select
    @iOKTemp = 0

begin tran
    if @bDeleteGameAccount = 1
    begin
        exec @iOKTemp = dbo.up_drop_user_info_old
            @strLoginE
        ,   1 --@bForceWithYou
        ,   @bReturned = 1
        if @iOKTemp <> 0
        begin    select @iOK = -101    goto fail_tran    end
    end

    delete a
    with (updlock)
    from dbo.EAGExperienceAccount
    as a
    with (updlock)
    where
        Login = @strLoginE_
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
    ,   2 --탈퇴
    ,   N''
    ,   N''
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
return
    @iOK


