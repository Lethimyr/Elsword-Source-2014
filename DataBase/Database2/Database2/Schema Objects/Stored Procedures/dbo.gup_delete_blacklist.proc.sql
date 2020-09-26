/*	sutamtwo
	2008-01-31
	블랙 리스트에 얻어오기
*/

CREATE PROCEDURE [dbo].[gup_delete_blacklist]
(
	@iMyUID		bigint	
,	@iBlockUID	bigint
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;

IF NOT EXISTS (SELECT * FROM dbo.GBlackList WITH (NOLOCK) WHERE MyUID = @iMyUID AND BlockUID = @iBlockUID)
	BEGIN	SELECT @iOK = -1	GOTO END_PROC	END


BEGIN TRAN

	DELETE FROM dbo.GBlackList
		WHERE MyUID = @iMyUID AND BlockUID = @iBlockUID

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN
			SELECT @iOK = -2
			GOTO FAIL_TRAN
		END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


