CREATE PROCEDURE [dbo].[up_drop_user_info]
	@strLogin_ [nvarchar](20),
	@bForceWithYou_ [bit] = 0,
	@bReturned [bit] = 0,
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.
*/

if @bForceWithYou_ = 0
and not exists
(
    select *
    from dbo.GlobalVariables
    where
        No = 7
    and Variable1 in ( 1 )
)
begin    select @iOK = -1    goto end_proc    end

declare
    @strLogin nvarchar(20)
,   @iLoginUID int
select
    @strLogin = N'kkurrung3_' + convert(nvarchar(10), LoginUID)
,   @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_

if @strLogin is null
begin    select @iOK = -2    goto end_proc    end

begin transaction

update a
with (updlock)
set
    a.Login = @strLogin
from dbo.Users
as a
with (updlock)
where
    Login = @strLogin_
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

delete a
with (updlock)
from dbo.NickNames
as a
with (updlock)
where
    Login = @strLogin
if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

delete a
with (updlock)
from dbo.UNGAUserNickname
as a
with (updlock)
where
    LoginUID = @iLoginUID
if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

update a
with (updlock)
set
    a.OwnerLogin = @strLogin
from dbo.GoodsObjectList
as a
with (updlock)
where
    OwnerLogin = @strLogin_
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

update a
with (updlock)
set
    a.OwnerLogin = @strLogin
from dbo.DurationItemObjectList
as a
with (updlock)
where
    OwnerLogin = @strLogin_
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -104    goto fail_tran    end

update a
with (updlock)
set
    a.Login = @strLogin
from dbo.Agreement
as a
with (updlock)
where
    Login = @strLogin_
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -105    goto fail_tran    end

delete a
with (updlock)
from dbo.UAGUserAuth
as a
with (updlock)
where
    LoginUID = @iLoginUID
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -106    goto fail_tran    end

insert into dbo.UAGUserAuth
with (updlock)
(
    LoginUID
,   AuthLevel
)
select
    @iLoginUID
,   -3
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -107    goto fail_tran    end

execute dbo.CSGA_couple_system_delete
    @iLoginUID

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
if @bReturned = 0
begin
    select
        @iOK
end
else if @bReturned = 1
begin
    return
        @iOK
end


