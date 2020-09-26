CREATE PROCEDURE [dbo].[PBGA_point_bag_select_reward]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081125. microcat.

{ call dbo.PBGA_point_bag_select_reward }

{ call dbo.PBGA_point_bag_select_reward }
인자없음 ㅋㅋ

n return ( @1, @2, @3, @4, @5 )
@1 ; AttendanceType tinyint
    1  ;주개근
    2 ; 월개근
@2 ; SupplyType tinyint
    1 ; 선택지급 ; 입력된 아이템 중 한개를 유저가 선택 지급
    2 ; 모두지급 ; 입력된 아이템을 해당 개근 조건이 충족되면 모두 지급
    3 ; 랜덤지급 ; 입력된 아이템 중 한개를 랜덤 지급
@3 ; ItemID int
@4 ; ItemType tinyint
@5 ; Factor int
*/

select
    AttendanceType
,   SupplyType
,   ItemID
,   ItemType
,   Factor
from dbo.PBGAPoingBagReward


