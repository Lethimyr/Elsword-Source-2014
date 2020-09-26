CREATE PROCEDURE [dbo].[PBDB_point_bag_select_reward]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081125. microcat.
*/

select
    a.AttendanceType
,   a.SupplyType
,   a.ItemID
,   b.GoodsName
,   a.ItemType
,   a.Factor
from dbo.PBGAPoingBagReward
as a
join dbo.GoodsInfoList
as b
on
    a.ItemID = b.GoodsID


