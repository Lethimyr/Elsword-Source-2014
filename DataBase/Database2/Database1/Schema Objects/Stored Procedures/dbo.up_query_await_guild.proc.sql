CREATE PROCEDURE [dbo].[up_query_await_guild]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090109. microcat.
*/

select
    GuildId
,   Name
,   LoadLogin
,   Explanation
,   Mark
,   RegDate
,   Point
,   NumMembers
,   NumMatches
,   NumWins
,   NumLooses
,   Approval
,   Email
,   cafe_url
from dbo.GuildInfo
where
    Approval = 0
order by
    RegDate desc


