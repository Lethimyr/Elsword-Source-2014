CREATE PROCEDURE [dbo].[SED_strong_enchant_select]
	@strLogin_ [nvarchar](20),
	@strFilter_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080918. microcat.
*/

declare
    @iLoginUID int
select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_
option (maxdop 1)

declare
    @strFilter nvarchar(22)
select
    @strFilter = N'%' + @strFilter_ + N'%'

select
    @strLogin_ as Login
,   a.ItemUID
,   a.ItemID
,   b.ItemName
,   b.Level
,   a.RegDate
from
(
    select
        ItemUID
    ,   NewItemID as ItemID
    ,   convert(nvarchar(19), RegDate, 120) as RegDate
    from dbo.SILStrongItem
    where
        LoginUID = @iLoginUID
)
as a
join
(
    select
        replace(GoodsName, N'%', N'p') as ItemName
    ,   StrongLevel as Level
    ,   GoodsID as ItemID
    from dbo.GoodsInfoList
    where
        GoodsName like @strFilter
)
as b
on
    a.ItemID = b.ItemID
order by
    a.ItemUID
,   a.Regdate
option (maxdop 1)


