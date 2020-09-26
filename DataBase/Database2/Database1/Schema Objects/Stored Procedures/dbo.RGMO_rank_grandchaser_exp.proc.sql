CREATE PROCEDURE [dbo].[RGMO_rank_grandchaser_exp]
	@iType_ [tinyint],
	@iCharType_ [tinyint],
	@bWeb_ [bit],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081229. microcat.
*/

declare
    @strType nvarchar(7)
,   @strCharType nvarchar(6)
select
    @strType
=   case @iType_
        when 4 then N'Exp'
    end
,   @strCharType
=   case @iCharType_
        when 0 then N''
        when 1 then N'Elesis'
        when 2 then N'Lire'
        when 3 then N'Arme'
        when 4 then N'Las'
        when 5 then N'Lyan'
        when 6 then N'Ronan'
        when 7 then N'Amy'
        when 8 then N'Jin'
    end

declare
    @strSQL nvarchar(4000)
select
    @strSQL = N'declare
    @Data' + @strType + @strCharType + N' table
(
    Rank int not null identity(1, 1)
,   LoginUID int not null
,   GuildID int not null
,   GuildMark int not null
,   GuildName nvarchar(12) not null
,   CharType tinyint not null
,   Nick nvarchar(24) not null
,   [Exp] bigint not null
,   primary key clustered
    (
        LoginUID
    )
)

insert into @Data' + @strType + @strCharType + N'
(
    LoginUID
,   GuildID
,   GuildMark
,   GuildName
,   CharType
,   Nick
,   [Exp]
)
select
    b.LoginUID
,   max(isnull(e.GuildID, -1))
,   max(isnull(f.Mark, -1))
,   max(isnull(f.Name, N''''))
,   '
+   case @iCharType_
        when 0 then
N'(
        select top 1
            CharType
        from dbo.Characters
        as g
        with (nolock)
        where
            max(b.Login) = g.Login
        order by
            [Exp] desc
        ,   CharType asc
    )'
        else convert(nvarchar(10), @iCharType_ - 1)
    end
+   N'
,   max(d.Nick)
,   sum(c.[Exp])
from dbo.Users
as b
with (updlock)
join'
+   case @iCharType_
        when 0 then
+   N'
(
    select
        Login
,       (sum([Exp]) + max([Exp]) * 2) / 3 as [Exp]
    from dbo.Characters
    with (nolock)
    group by
        Login
)
as c'
        else N' dbo.Characters
as c
with (nolock)'
    end
+   N'
on
    b.Login = c.Login'
+   case @iCharType_
        when 0 then N''
        else N'
and c.CharType = ' + convert(nvarchar(10), @iCharType_ - 1)
    end
+   N'
join dbo.NickNames
as d
with (nolock)
on
    b.Login = d.Login
left outer join dbo.GuildUser
as e
with (nolock)
on
    b.Login = e.Login
left outer join dbo.GuildInfo
as f
with (nolock)
on
    e.GuildID = f.GuildID
where
    not exists
(
    select *
    from dbo.UAGUserAuth
    as g
    with (nolock)
    where
        b.LoginUID = g.LoginUID
    and g.AuthLevel not between -2 and 0
)
and not exists
(
    select *
    from dbo.RCRRankCharacterAbuse
    as h
    with (nolock)
    where
        b.LoginUID = h.LoginUID
    and h.Abused = 1
)
group by
    b.LoginUID
order by
    7 desc
,   1 asc
option (maxdop 1)

insert into dbo.RGRRankGrandchaser' + @strType + N'_old
with (updlock)
(
    TabType
,   Rank
,   LoginUID
,   GuildID
,   GuildMark
,   GuildName
,   CharType
,   Nick
,   [Exp]
)
select
    ' + convert(nvarchar(10), @iCharType_) + N'
,   Rank
,   LoginUID
,   GuildID
,   GuildMark
,   GuildName
,   CharType
,   Nick
,   [Exp]
from @Data' + @strType + @strCharType + N'
option (maxdop 1)

execute Web.dbo.sp_executesql
N''
    truncate table dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old
''

if @bWeb = 1
begin
    execute Web.dbo.sp_executesql
    N''
        drop index dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old.IXU_RGRRankGrandchaser_Nick
        drop index dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old.IXU_RGRRankGrandchaser_LoginUID
        drop index dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old.IXC_RGRRankGrandchaser_Rank
    ''

    insert into Web.dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old
    with (updlock)
    (
        Rank
    ,   LoginUID
    ,   GuildID
    ,   GuildMark
    ,   GuildName
    ,   CharType
    ,   Nick
    ,   [Exp]
    )
    select
        Rank
    ,   LoginUID
    ,   GuildID
    ,   GuildMark
    ,   GuildName
    ,   CharType
    ,   Nick
    ,   [Exp]
    from @Data' + @strType + @strCharType + N'
    option (maxdop 1)

    execute Web.dbo.sp_executesql
    N''
        create clustered index IXC_RGRRankGrandchaser_Rank on dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old (Rank) with fillfactor = 80
        create unique index IXU_RGRRankGrandchaser_Nick on dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old (Nick) with fillfactor = 80
        create unique index IXU_RGRRankGrandchaser_LoginUID on dbo.RGRRankGrandchaser' + @strType + @strCharType + N'_old (LoginUID) with fillfactor = 80

        execute dbo.sp_rename ''''RGRRankGrandchaser' + @strType + @strCharType + N''''', ''''RGRRankGrandchaser' + @strType + @strCharType + N'_temp''''
        execute dbo.sp_rename ''''RGRRankGrandchaser' + @strType + @strCharType + N'_old'''', ''''RGRRankGrandchaser' + @strType + @strCharType + N'''''
        execute dbo.sp_rename ''''RGRRankGrandchaser' + @strType + @strCharType + N'_temp'''', ''''RGRRankGrandchaser' + @strType + @strCharType + N'_old''''
    ''
end'

execute dbo.sp_executesql
    @strSQL
,   N'@bWeb bit'
,   @bWeb_


