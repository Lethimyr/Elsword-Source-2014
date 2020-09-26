CREATE PROCEDURE [dbo].[eup_santa_check]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081222. microcat.
*/

declare
    @strLogin nvarchar(20)
,   @sdtFirstLogin smalldatetime
,   @sdtLastLogin smalldatetime
,   @sdtNow smalldatetime
select
    @strLogin = Login
,   @sdtFirstLogin = FirstLogin
,   @sdtLastLogin = LastLogin
,   @sdtNow = current_timestamp
from dbo.Users
where
    LoginUID = @iLoginUID_

if @strLogin is null
begin    select @iOK = -1    goto end_proc    end

declare
    @Santa table
(
    EventUID int not null
,   UserType tinyint not null
,   Comment nvarchar(100) not null
,   ItemID int not null
)


insert into @Santa
(
    EventUID
,   UserType
,   Comment
,   ItemID
)
select
    a.EventUID
,   a.UserType
,   a.Comment
,   a.ItemID
from dbo.ESanta
as a
left outer join dbo.ESantaLimitDate
as d
on
    a.EventUID = d.EventUID
where
    exists
(
    select *
    from dbo.ESantaDate
    as b
    where
        a.EventUID = b.EventUID
        and b.StartDate <= @sdtNow
        and @sdtNow < b.EndDate
)
and not exists
(
    select *
    from dbo.ESantaUser
    as c
    with (nolock)
    where
        a.EventUID = c.EventUID
        and c.LoginUID = @iLoginUID_
)
and
(
    a.UserType = 8 
or  a.UserType =
    case
        when @sdtFirstLogin = @sdtLastLogin then 1
        when d.LimitDate is null then 8
        when @sdtLastLogin < d.LimitDate then 2
        else 4
    end
)

if not exists
(
    select *
    from @Santa
)
begin    select @iOK = -2    goto end_proc    end

declare
    @strItemList nvarchar(4000)
select
    @strItemList = N''
select
    @strItemList
=   @strItemList
+   cast(a.ItemID as nvarchar(10))
+   N','
+   cast(a.Factor as nvarchar(10))
+   N','
from dbo.ESantaItem
as a
where
    exists
(
    select *
    from @Santa
    as b
    where
        a.EventUID = b.EventUID
    and a.UserType = b.UserType
)

if @strItemList = N''
begin    select @iOK = -3    goto end_proc    end

begin transaction

execute @iOK = dbo.up_insert_item
    @strLogin
,   @strItemList
,   @bReturn_ = 0

if @iOK <> 0
begin    select @iOK = -101    goto fail_tran    end
    
insert into dbo.ESantaUser
with (updlock)
(
    EventUID
,   LoginUID
,   RegDate
)
select
    EventUID
,   @iLoginUID_
,   @sdtNow
from @Santa
group by
    EventUID

if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    Comment
,   ItemID
from @Santa
where
    @iOK = 0


