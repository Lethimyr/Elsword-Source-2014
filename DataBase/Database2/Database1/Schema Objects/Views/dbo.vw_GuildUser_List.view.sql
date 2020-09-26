CREATE VIEW [dbo].[vw_GuildUser_List]
AS 
select
    a.grade
,   b.GuildId
,   a.Login
,   c.Exp0
,   c.Exp1
,   c.Exp2
,   c.Exp3
,   c.Exp4
,   c.Exp5
,   c.Exp6
,   c.Exp7
,   b.Point
,   b.UserLevel
from dbo.Users as a
with (nolock)
join dbo.GuildUser as b
with (nolock)
on a.Login = b.Login
join
(
    select
        login
    ,   Exp0 = sum(case CharType when 0 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp1 = sum(case CharType when 1 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp2 = sum(case CharType when 2 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp3 = sum(case CharType when 3 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp4 = sum(case CharType when 4 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp5 = sum(case CharType when 5 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp6 = sum(case CharType when 6 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    ,   Exp7 = sum(case CharType when 7 then cast(case when [Exp] > 1865394200 then 1865394200 else [Exp] end as int) else 0 end)
    from dbo.Characters
    with (nolock)
    group by Login
)  as c
on a.login = c.login


