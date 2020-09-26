CREATE PROCEDURE [dbo].[gup_update_Attribute]
	@iItemUID	bigint
,	@iSlotNo	Tinyint
,	@iAttribute	tinyint
,	@iOK		int = 0

AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN
	IF NOT EXISTS (SELECT * FROM dbo.GItemAttribute WITH (NOLOCK) WHERE ItemUID = @iItemUID AND SlotNo = @iSlotNo)
		BEGIN
			INSERT INTO dbo.GItemAttribute (ItemUID, SlotNo, Attribute)
				SELECT @iItemUID, @iSlotNo, @iAttribute
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
			GOTO COMMIT_TRAN
		END

		UPDATE dbo.GItemAttribute
			SET Attribute	= @iAttribute
				WHERE ItemUID = @iItemUID AND SlotNo = @iSlotNo
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1 
		BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END


COMMIT_TRAN:
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


