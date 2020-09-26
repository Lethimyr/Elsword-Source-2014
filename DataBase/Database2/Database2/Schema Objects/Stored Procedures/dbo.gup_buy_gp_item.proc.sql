CREATE PROCEDURE [dbo].[gup_buy_gp_item]
    @iUnitUID       [bigint],
    @iItemID        [int],
    @iPrice         [int],
    @iPvPPoint      [int],
    @iPeriodType    [tinyint],
    @iQuantity      [smallint],
    @iOK            [int] = 0,
    @iItemUID       [bigint] = 0
AS

SET NOCOUNT ON;

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

-- 수량성 아이템인데 가지고 있는 경우에는 수량만 더해진다.
--IF @iPeriodType = 2
--BEGIN
	--DECLARE @InternalItemUID TABLE ( UID bigint Primary Key Not NULL )
	--INSERT INTO @InternalItemUID(UID) 
	--	SELECT ItemUID FROM dbo.GItem WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND ItemID = @iItemID AND Deleted = 0
  --  SELECT @iItemUID = ItemUID FROM dbo.GItem WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND ItemID = @iItemID AND Deleted = 0

	
--END

DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

BEGIN TRAN


    INSERT INTO dbo.GItem( UnitUID, ItemID, RegDate, DelDate )
    SELECT @iUnitUID, @iItemID, @sdtNow, @sdtNow
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN
	    SELECT @iOK = -5
	    GOTO FAIL_TRAN
    END
    SELECT @iItemUID = @@IDENTITY

    
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iItemUID


