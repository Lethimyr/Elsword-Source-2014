CREATE PROCEDURE [dbo].[gup_tool_insert_item]
    @iUnitUID       [bigint],
    @iItemID        [int],
    @iQuantity      [smallint],	
	@strSession		[nvarchar](20),
	@strIP			[nvarchar](15),
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

DECLARE @sdtNow			smalldatetime
,		@iPeriodType	tinyint
,		@iEndurance		smallint
SELECT  @sdtNow = GetDate()


select @iPeriodType = Period, @iEndurance = Endurance
	from dbo.gitemcode
		where itemcode = @iItemID



BEGIN TRAN
	INSERT INTO dbo.GItem( UnitUID, ItemID, InventoryCategory, SlotID, RegDate, DelDate )
		SELECT @iUnitUID, @iItemID, 0, 0, @sdtNow, @sdtNow

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN
			SELECT @iOK = -11 -- GItem insert Error
			GOTO FAIL_TRAN
		END

	SELECT @iItemUID = @@IDENTITY

-- 수량성 아이템이면,
    IF @iPeriodType = 2
    BEGIN
        INSERT INTO dbo.GItemQuantity( ItemUID, Quantity )
			SELECT @iItemUID, @iQuantity
        IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN
				SELECT @iOK = -12 -- 수량성 아이템 insert Error
				GOTO FAIL_TRAN
			END
    END
-- 내구도 아이템이면,
	IF @iPeriodType = 1
    BEGIN
        INSERT INTO dbo.GItemEndurance( ItemUID, Endurance )
			SELECT @iItemUID, @iEndurance
        IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN
				SELECT @iOK = -13 -- 내구도 아이템 insert Error
				GOTO FAIL_TRAN
			END
    END
    

-- 로그 남기기
	insert into dbo.GItemToolHistory(UnitUID,Itemuid,InsOrDel,session,ip,regdate)
		select @iUnitUID, @iItemUID, 0, @strSession, @strIP, @sdtNow
		    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN
					SELECT @iOK = -14 -- Log insert Error
					GOTO FAIL_TRAN
				END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


