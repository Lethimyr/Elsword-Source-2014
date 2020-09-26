CREATE PROCEDURE [dbo].[CGDB_character_gameinfo_select]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090107. microcat.
*/

declare
    @Data table
(
    No int not null identity(0, 1)
,   CharType tinyint not null
,   Promotion tinyint not null
,   [Exp] bigint not null
,   Level tinyint not null
,   Win int not null
,   Lose int not null
,   Grade tinyint not null
)
insert into @Data
(
    CharType
,   Promotion
,   [Exp]
,   Level
,   Win
,   Lose
,   Grade
)
select
    CharType
,   Promotion
,   [Exp]
,   Level
,   Win
,   Lose
,   dbo.zdGrade_Win(Win)
from dbo.Characters
where
    Login = @strLogin_
order by
    CharType
option (maxdop 1)

select
    No
,   CharType
,   Promotion
,   [Exp]
,   Level
,   Win
,   Lose
,   Grade
from @Data


