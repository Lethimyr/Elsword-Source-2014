CREATE PROCEDURE [dbo].[TCDB_title_collection_user_select]
	@strLogin_ [nvarchar](20)
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080930. microcat.
*/

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_

select
    @strLogin_ as Login
,   a.MainMissionID
,   b.SubMissionID
,   b.TCCount as [Count]
,   convert(nvarchar(16), a.StartDate, 120) as StartDate
,   convert(nvarchar(16), a.EndDate, 120) as EndDate
,   case a.Status when 1 then 'Progress' when 2 then 'Clear' end as Status
from
(
    select
        LoginUID
    ,   MainMissionID
    ,   StartDate
    ,   EndDate
    ,   Status
    from dbo.TCGTitleCollectionMain
    where
        LoginUID = @iLoginUID
)
as a
join
(
    select
        LoginUID
    ,   MainMissionID
    ,   SubMissionID
    ,   TCCount
    from dbo.TCGTitleCollectionSub
    where
        LoginUID = @iLoginUID
)
as b
on
    a.LoginUID = b.LoginUID
and a.MainMissionID = b.MainMissionID
option (maxdop 1)


