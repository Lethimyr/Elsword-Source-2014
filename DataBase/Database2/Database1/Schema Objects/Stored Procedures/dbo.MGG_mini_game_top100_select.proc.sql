CREATE PROCEDURE [dbo].[MGG_mini_game_top100_select]
AS
set nocount on

declare
    @Result table
(
    LoginUID int not null
,   GameMode tinyint not null
,   CharType tinyint not null
,   Point int not null
,   Login nvarchar(20) collate database_default not null
,   Nick nvarchar(24) collate database_default not null
,   primary key clustered
    (
        LoginUID
    ,   GameMode
    )
)
insert into @Result
(
    LoginUID
,   GameMode
,   CharType
,   Point
,   Login
,   Nick
)
select
    LoginUID
,   GameMode
,   CharType
,   Point
,   N''
,   N''
from
(
    select top 100
        LoginUID
    ,   GameMode
    ,   CharType
    ,   Point
    from dbo.MGGMiniGame
    with (nolock)
    where
        GameMode = 0
    order by
        Point desc
    ,   LoginUID
    union all
    select top 100
        LoginUID
    ,   GameMode
    ,   CharType
    ,   Point
    from dbo.MGGMiniGame
    with (nolock)
    where
        GameMode = 1
    order by
        Point desc
    ,   LoginUID
    union all
    select top 100
        LoginUID
    ,   GameMode
    ,   CharType
    ,   Point
    from dbo.MGGMiniGame
    with (nolock)
    where
        GameMode = 2
    order by
        Point desc
    ,   LoginUID
) as a
if @@error <> 0
begin    goto end_proc    end

update a
set
    a.Login = b.Login
from @Result
as a
join dbo.Users
as b
with (nolock)
on
    a.LoginUID = b.LoginUID
option (maxdop 1)
if @@error <> 0
begin    goto end_proc    end

update a
set
    a.Nick = b.Nick
from @Result
as a
join dbo.NickNames
as b
with (nolock)
on
    a.Login = b.Login
option (maxdop 1)
if @@error <> 0
begin    goto end_proc    end

end_proc:
select
    GameMode
,   LoginUID
,   Nick
,   CharType
,   Point
from @Result
option (maxdop 1)


