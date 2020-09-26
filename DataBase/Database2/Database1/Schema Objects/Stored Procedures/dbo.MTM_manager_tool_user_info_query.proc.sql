CREATE PROCEDURE [dbo].[MTM_manager_tool_user_info_query]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
as
set nocount on

declare
    @iLoginUID int
,   @iGamePoint int
,   @iAuthLevel int
,   @iStatus int
select
    @iGamePoint = 0
,   @iAuthLevel = 0
,   @iStatus = 0
select
    @iLoginUID = LoginUID
,   @iGamePoint = GamePoint
from dbo.Users
with (nolock)
where
    Login = @strLogin_
if @iLoginUID is null
begin    select @iOK = -2    goto end_proc    end

select
    @iAuthLevel = AuthLevel
from dbo.UAGUserAUth
with (nolock)
where
    LoginUID = @iLoginUID
select
    @iStatus = Status
from dbo.UAGUserAuthStatus
with (nolock)
where
    LoginUID = @iLoginUID

end_proc:
select
    @iOK
,   @iGamePoint
,   @iAuthLevel
,   @iStatus


