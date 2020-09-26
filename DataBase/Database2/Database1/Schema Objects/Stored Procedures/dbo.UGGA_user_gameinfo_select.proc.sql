CREATE PROCEDURE [dbo].[UGGA_user_gameinfo_select]
	@strLogin_ [nvarchar](20),
	@strIPs [nvarchar](15),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081107. microcat.

{ call dbo.UGGA_user_gameinfo_select ( N'%s', N'%s' ) }

{ call dbo.UGGA_user_gameinfo_select ( @1, @2 ) }
@1 ; Login nvarchar(20)
@2 ; IPAddress nvarchar(15)

1 return ( @1, @2, @3, @4 )
@1 ; Sex char(1)
@2 ; LoginUID int
@3 ; Type tinyint ; 0(첫접속 아님), 1(기존유저 오늘 첫접속), 2(신규유저 계정생성후 첫접속)
@4 ; FirstLogin smalldatetime
*/

declare
    @iLoginUID int

begin transaction

update a
with (updlock)
set
    IPAddress = @strIPs
,   Connecting = 1
,   @iLoginUID = LoginUID
from dbo.Users
as a
with (updlock)
where
    Login = @strLogin_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

execute [Log].dbo.IPMO_internet_protocol_insert
    @iLoginUID
,   @strIPs
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    sex
,   LoginUID
,   gamePoint
,   case
        when LastLogin < convert(nvarchar(10), getdate(), 120) then 1
        when FirstLogin = LastLogin then 2
        else 0
    end
,   FirstLogin
from dbo.Users
where
    Login = @strLogin_
option (maxdop 1)


