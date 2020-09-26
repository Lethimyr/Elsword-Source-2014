CREATE PROCEDURE [dbo].[CMD_connecting_message_user_insert]
	@strData_ [nvarchar](4000),
	@iMessageID_ [int],
	@sdtLimitDate_ [smalldatetime],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2008년 3월 28일
사용 : dbot에서 유저 입력합니다.
*/
set nocount on

if not exists
(
    select *
    from dbo.CMGConnectingMessage
    with (nolock)
    where
        MessageID = @iMessageID_
)
begin    select @iOK = -1    goto end_proc    end

if @sdtLimitDate_ <= convert(nvarchar(10), dateadd(dd, -1, getdate()), 120)
begin    select @iOK = -2    goto end_proc    end

declare
    @strToken nvarchar(1)
,   @iSplit tinyint
select
    @strToken = N','
,   @iSplit = 1

declare @result table
(
    No smallint not null identity(0, 1) primary key clustered
,   Data nvarchar(4000) not null
)

insert into @result
(
    Data
)
select
    substring
    (
        @strData_
    ,   min(a.No)
    ,   charindex(@strToken, @strData_ + @strToken, min(a.No)) - min(a.No)
    )
from dbo.TDNDataNo as a
with (nolock)
where a.No <= len(@strData_)
group by
    charindex(@strToken, @strData_ + @strToken, a.No)
order by
    charindex(@strToken, @strData_ + @strToken, a.No)
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -3    goto end_proc    end

declare @data table
(
    Login nvarchar(20) collate database_default not null
,   LoginUID int null
,   primary key clustered
    (
        Login
    )
)

insert into @Data
(
    Login
)
select
    max(case when No % @iSplit = 0 then Data end)
from @result
group by No / @iSplit
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -4    goto end_proc    end

update a
set
    a.LoginUID = b.LoginUID
from @data as a
join dbo.Users as b
with (nolock)
on a.Login = b.Login
option (maxdop 1)

if exists
(
    select *
    from @data
    where LoginUID is null
)
begin    select @iOK = -5    goto end_proc    end

begin tran
    insert into dbo.CMGConnectingMessageUser
    with (updlock)
    (
        LoginUID
    ,   MessageID
    ,   LimitDate
    ,   Readed
    )
    select
        LoginUID
    ,   @iMessageID_
    ,   @sdtLimitDate_
    ,   0
    from @data
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK as OK


