CREATE PROCEDURE [dbo].[RCR_rank_character_schedule]
AS
set nocount on

declare
    @sdtDate smalldatetime
select
    @sdtDate = convert(nvarchar(10), dateadd(dd, -1, getdate()), 120)

if exists
(
    select *
    from dbo.RCRRankCharacter
    with (nolock)
    where RegDate = @sdtDate
)
begin    goto end_proc    end

begin tran
    insert into dbo.RCRRankCharacter
    with (updlock)
    (
        RegDate
    ,   LoginUID
    ,   CharType
    ,   Win
    ,   Lose
    )
    select
        @sdtDate
    ,   LoginUID
    ,   CharType
    ,   Win
    ,   Lose
    from dbo.RCRRankCharacterCollect
    with (updlock)
    option (maxdop 1)
    if @@error <> 0
    begin    goto fail_tran    end

    truncate table dbo.RCRRankCharacterCollect
    if @@error <> 0
    begin    goto fail_tran    end
commit tran

declare @Abuse table
(
    LoginUID int not null
,   RegDate smalldatetime not null
,   primary key clustered
    (
        LoginUID
    ,   RegDate
    )
)
insert into @Abuse
(
    LoginUID
,   RegDate
)
select
    a.LoginUID
,   a.RegDate
from dbo.RCRRankCharacter
as a
with (nolock)
join dbo.RCRRankCharacterAbuse
as b
with (nolock)
on
    a.LoginUID = b.LoginUID
and a.RegDate = b.RegDate
and a.RegDate = @sdtDate
and 100 <= a.Win + a.Lose
and a.Win + a.Lose <= a.Win / 0.9
and b.RegDate = @sdtDate
and b.Abused = 0
where
    not exists
(
    select *
    from dbo.RCRRankCharacterAbuse
    as c
    with (nolock)
    where
        b.LoginUID = c.LoginUID
    and c.RegDate < @sdtDate
    and c.Abused = 1
)
group by
    a.RegDate
,   a.LoginUID
option (maxdop 1)

begin tran
    update a
    with (updlock)
    set
        a.Abused = 1
    from dbo.RCRRankCharacterAbuse
    as a
    with (updlock)
    join @Abuse
    as b
    on
        a.LoginUID = b.LoginUID
    and a.RegDate = b.RegDate
    option (maxdop 1)
    if @@error <> 0
    begin    goto fail_tran    end

    insert into dbo.CMGConnectingMessageUser
    with (updlock)
    (
        LoginUID
    ,   MessageID
    ,   LimitDate
    ,   Readed
    )
    select
        a.LoginUID
    ,   2008051901
    ,   dateadd(dd, -1, dateadd(mm, 1, convert(nvarchar(8), dateadd(dd, 1, @sdtDate), 120) + N'01'))
    ,   0
    from @Abuse
    as a
    left outer join
    (
        select
            LoginUID
        from dbo.CMGConnectingMessageUser
        with (updlock)
        where
            MessageID = 2008051901
    )
    as b
    on
        a.LoginUID = b.LoginUID
    where
        b.LoginUID is null
    option (maxdop 1)
    if @@error <> 0
    begin    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:


