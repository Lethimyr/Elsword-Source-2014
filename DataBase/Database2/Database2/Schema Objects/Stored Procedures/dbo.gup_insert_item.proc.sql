CREATE PROCEDURE [dbo].[gup_insert_item]
    @iUnitUID       [bigint],
    @iItemID        [int],
    @iPeriodType    [tinyint],
    @iQuantity      [smallint],	
	@iEndurance		[int],
	@iPeriod		[int],
	@iElevel		[smallint] = 0,
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

DECLARE @sdtNow smalldatetime
,		@sdtEndDate smalldatetime
SELECT  @sdtNow = GetDate()


BEGIN TRAN
	INSERT INTO dbo.GItem( UnitUID, ItemID, InventoryCategory, SlotID, RegDate, DelDate )
	SELECT @iUnitUID, @iItemID, 0, 0, @sdtNow, @sdtNow
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
	BEGIN
		SELECT @iOK = -11
		GOTO FAIL_TRAN
	END
	SELECT @iItemUID = @@IDENTITY


    IF @iPeriodType = 0 AND @iPeriod > 0
    BEGIN
		SELECT	@sdtEndDate = dateadd( dd, @iPeriod, @sdtNow )
	        INSERT INTO dbo.GItemPeriod( ItemUID, Period, ExpirationDate )
			    SELECT @iItemUID, @iPeriod, @sdtEndDate
					IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					    BEGIN	SELECT @iOK = -12	GOTO FAIL_TRAN	END
	END


	IF @iPeriodType = 1
    BEGIN
        INSERT INTO dbo.GItemEndurance( ItemUID, Endurance )
        SELECT @iItemUID, @iEndurance
        IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
	    BEGIN
		    SELECT @iOK = -13
		    GOTO FAIL_TRAN
	    END
    END


    IF @iPeriodType = 2
    BEGIN
        INSERT INTO dbo.GItemQuantity( ItemUID, Quantity )
        SELECT @iItemUID, @iQuantity
        IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
	    BEGIN
		    SELECT @iOK = -12
		    GOTO FAIL_TRAN
	    END
    END

    

	IF @iElevel > 0 
		BEGIN
			INSERT INTO dbo.GItemEnchant (ItemUID, Elevel)
				SELECT @iItemUID, @iElevel
			    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN	SELECT @iOK = -14	GOTO FAIL_TRAN	END
		END
	
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iItemUID, @sdtEndDate


