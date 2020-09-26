CREATE PROCEDURE [dbo].[gup_insert_recommend]
	@NewUID		bigint
,	@OldNick	nvarchar(16)
,	@iOK		int = 0
AS
SET NOCOUNT ON;


IF NOT EXISTS (SELECT * FROM dbo.GUnitNickName WITH (NOLOCK) WHERE NickName = @OldNick)
BEGIN
	SELECT	@iOK = -1	GOTO END_PROC
END


DECLARE @sdtNow	smalldatetime
,		@iOldUID int

SELECT	@sdtNow	= GETDATE()
SELECT	@iOldUID = UnitUID
	FROM dbo.GUnitNickName	WITH (NOLOCK)
		WHERE NickName = @OldNick

BEGIN TRAN
	
	INSERT INTO dbo.GRecommend (OldUID, NewUID, RegDate)
		SELECT @iOldUID, @NewUID, @sdtNow

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT	@iOK = -2	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iOldUID


