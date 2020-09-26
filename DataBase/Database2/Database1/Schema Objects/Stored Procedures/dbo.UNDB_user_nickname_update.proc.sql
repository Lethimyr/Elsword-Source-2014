CREATE PROCEDURE [dbo].[UNDB_user_nickname_update]
	@strLogin_ [nvarchar](20),
	@strNewNickname_ [nvarchar](24),
	@iOK [int] = 0,
	@strMsg [nvarchar](4000) = N''
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081230. microcat.
*/

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_

if @iLoginUID is null
begin
    select
        @iOK = -1
    ,   @strMsg = N'Not found account.'
    goto end_proc
end

declare
    @strOldNickname nvarchar(24)
select
    @strOldNickname = Nick
from dbo.NickNames
where
    Login = @strLogin_

if @strOldNickname is null
begin
    select
        @iOK = -2
    ,   @strMsg = N'The Nick is not registered.'
    goto end_proc
end

if exists
(
    select *
    from dbo.Nicknames
    where
        Nick = @strNewNickname_
)
begin
    select
        @iOK = -3
    ,   @strMsg = N'Already used.'
    goto end_proc
end

begin transaction

update a
with (updlock)
set
    Nick = @strNewNickname_
,   RegDate = current_timestamp
from dbo.NickNames
as a
with (updlock)
where
    Login = @strLogin_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

execute dbo.UNMO_user_nickname_merge
    @iLoginUID
,   @strNewNickname_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

insert into dbo.LogNicknames
with (updlock)
(
    Login
,   OldNick
,   NewNick
)
select
    @strLogin_
,   @strOldNickname
,   @strNewNickname_

if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

commit transaction

select
    @strMsg
=   N'Succeeded( '
+   @strOldNickname
+   N' -> '
+   @strNewNickname_
+   N' )'

goto end_proc

fail_tran:
rollback tran
select
    @strMsg = N'Transaction Error'

end_proc:
select
    @iOK as OK
,   @strMsg as Msg


