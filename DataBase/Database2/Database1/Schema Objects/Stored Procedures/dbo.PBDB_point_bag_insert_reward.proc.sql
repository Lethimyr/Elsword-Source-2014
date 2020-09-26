CREATE PROCEDURE [dbo].[PBDB_point_bag_insert_reward]
	@iAttendanceType_ [tinyint],
	@iSupplyType_ [tinyint],
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081125. microcat.
*/

if exists
(
    select *
    from dbo.PBGAPoingBagReward
    where
        AttendanceType = @iAttendanceType_
    and SupplyType = @iSupplyType_
    and ItemID = @iItemID_
)
begin    select @iOK = -1    goto end_proc    end

declare
    @iItemType tinyint
select
    @iItemType = ItemType
from dbo.GoodsInfoList
where
    GoodsID = @iItemID_

if @iItemType is null
begin    select @iOK = -2    goto end_proc    end

begin transaction

insert into dbo.PBGAPoingBagReward
with (updlock)
(
    AttendanceType
,   SupplyType
,   ItemID
,   ItemType
,   Factor
)
select
    @iAttendanceType_
,   @iSupplyType_
,   @iItemID_
,   @iItemType
,   @iFactor_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


