CREATE PROCEDURE [dbo].[PBDB_point_bag_delete_reward]
	@iAttendanceType_ [tinyint],
	@iSupplyType_ [tinyint],
	@iItemID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081125. microcat.
*/

if not exists
(
    select *
    from dbo.PBGAPoingBagReward
    where
        AttendanceType = @iAttendanceType_
    and SupplyType = @iSupplyType_
    and ItemID = @iItemID_
)
begin    select @iOK = -1    goto end_proc    end

begin transaction

delete a
with (updlock)
from dbo.PBGAPoingBagReward
as a
with (updlock)
where
    AttendanceType = @iAttendanceType_
and SupplyType = @iSupplyType_
and ItemID = @iItemID_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


