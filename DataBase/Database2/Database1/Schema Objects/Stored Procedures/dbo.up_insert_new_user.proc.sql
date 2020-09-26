CREATE PROCEDURE [dbo].[up_insert_new_user]
	@strLogin_ [nvarchar](20),
	@strPassword_ [nvarchar](32),
	@bSex_ [bit],
	@bReturned [bit] = 0,
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

if exists
(
    select *
    from dbo.Users
    where
        Login = @strLogin_
)
begin    select @iOK = -1    goto end_proc    end

begin transaction

insert into dbo.Users
with (updlock)
(
    Login
,   passwd
,   sex
)
select
    @strLogin_
,   @strPassword_
,   @bSex_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

if not exists
(
    select *
    from dbo.GlobalVariables
    where
        No = 7
    and Variable1 in ( 1 )
)
begin    goto end_proc    end

begin transaction

insert into dbo.DurationItemObjectList
with (updlock)
(
    OwnerLogin
,   BuyerLogin
,   GoodsID
,   Duration
)
select
    @strLogin_
,   N'__mc__20080313__00__'
,   a.ItemID
,   1
from
(
    select --하이
        71800 as ItemID
    union all
    select --아놔
        72110
    union all
    select --하하
        72190
    union all
    select --흑흑
        72210
    union all
    select --사랑해
        72260
)
as a
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

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


