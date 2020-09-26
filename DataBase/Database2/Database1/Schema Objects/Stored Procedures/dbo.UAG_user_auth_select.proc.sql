CREATE PROCEDURE [dbo].[UAG_user_auth_select]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2008년 3월 26일
사용 : 유저 인증 상태 점검
*/
set nocount on

declare
    @iAuthLevelA int
,   @iAuthLevelB int
,   @sdtDate smalldatetime
,   @iStatus int
,   @bReset bit
select
    @iAuthLevelA = 0
,   @iAuthLevelB = 0
,   @sdtDate = getdate()
,   @iStatus = 0
,   @bReset = 1

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_

select
    @iAuthLevelA = AuthLevel
,   @iAuthlevelB = Authlevel
from dbo.UAGUserAuth
with (nolock)
where
    LoginUID = @iLoginUID_

select
    @bReset = variable1
from dbo.GlobalVariables
with (nolock)
where No = 8

if exists
(
    select *
    from dbo.UAGUserAuthDate
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and LimitDate < @sdtDate
    and @iAuthLevelA between -3 and -1
)
begin
    begin tran
        if @bReset = 1
        and @iAuthLevelA = -3
        begin
            update a
            with (updlock)
            set
                GamePoint = 200
            from dbo.Users as a
            with (updlock)
            where LoginUID = @iLoginUID_
            if @@error <> 0
            begin    goto fail_tran    end

            update a
            with (updlock)
            set
                [Exp] = 2000
            ,   Level = 0
            from dbo.Characters as a
            with (updlock)
            where Login = @strLogin
            if @@error <> 0
            begin    goto fail_tran    end
        end

        delete a
        with (updlock)
        from dbo.UAGUserAuth as a
        with (updlock)
        where
            LoginUID = @iLoginUID_
        if @@error <> 0
        begin    goto fail_tran    end
    commit tran

    select @iAuthLevelB = 0
end

select
    @sdtDate = LimitDate
from dbo.UAGUserAuthDate 
with (nolock)
where
    LoginUID = @iLoginUID_
and @iAUthLevelA between -2 and -1

select
    @iStatus = Status
from dbo.UAGUserAuthStatus
with (nolock)
where
    LoginUID = @iLoginUID_
and @iAuthLevelA between -2 and -1

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iAuthLevelA
,   @iAuthLevelB
,   @sdtDate
,   @iStatus


