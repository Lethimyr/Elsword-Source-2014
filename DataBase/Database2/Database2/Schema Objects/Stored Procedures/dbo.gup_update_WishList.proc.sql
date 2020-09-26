CREATE PROCEDURE [dbo].[gup_update_WishList]
	@iUserUID	bigint 
,	@iSlotNo	smallint
,	@iItemID	int
,	@iOK		int = 0

AS
SET NOCOUNT ON;

BEGIN TRAN

	IF NOT EXISTS (SELECT * FROM dbo.GItemWishList WITH (NOLOCK) WHERE UserUID = @iUserUID AND SlotNo = @iSlotNo)
		BEGIN
			INSERT INTO dbo.GItemWishList (UserUID, SlotNo, ItemID)
				SELECT @iUserUID, @iSlotNo, @iItemID
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END
		END

	ELSE
		BEGIN
			UPDATE dbo.GItemWishList WITH (UPDLOCK)
				SET ItemID = @iItemID
					WHERE UserUID = @iUserUID
						AND SlotNo = @iSlotNo
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN    SELECT @iOK = -2    GOTO FAIL_TRAN    END
		END


COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


