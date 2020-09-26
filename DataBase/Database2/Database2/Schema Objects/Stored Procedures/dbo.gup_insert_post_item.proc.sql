CREATE PROCEDURE [dbo].[gup_insert_post_item]
	@FromUnitUID	bigint	
,	@ToUnitUID		bigint	
,	@Quantity		int		
,	@ScType			tinyint	
,	@ScIndex		bigint	
,	@Message		nvarchar(800)
,	@iOK			int = 0
AS
SET NOCOUNT ON;

DECLARE @sdtNow		smalldatetime
,		@iPostNo	bigint
SELECT	@sdtNow	= GETDATE()

BEGIN TRAN
	
	INSERT INTO dbo.GPost (FromUnitUID,	ToUnitUID, Quantity, ScType, ScIndex, RegDate, DelDate,	Message)
		SELECT @FromUnitUID, @ToUnitUID, @Quantity, @ScType, @ScIndex, @sdtNow, @sdtNow, @Message
	SELECT @iPostNo = @@IDENTITY  
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT	@iOK = -1	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iPostNo, @sdtNow


