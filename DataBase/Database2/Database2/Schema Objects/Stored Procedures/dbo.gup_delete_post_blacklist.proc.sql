/*	sutamtwo
	2008-09-29
	블랙 리스트에 얻어오기
*/

create PROCEDURE [dbo].[gup_delete_post_blacklist]
(
	@iMyUID		bigint	
,	@iBlockNickName nvarchar(16)
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;

DECLARE	@iBlockUID	bigint
SELECT	@iBlockUID = unitUID
	FROM dbo.GUnitNickName	WITH (NOLOCK)
		WHERE NickName = @iBlockNickName

IF @iBlockUID IS NULL
	BEGIN
		SELECT @iOK = -1
		GOTO END_PROC
	END

IF NOT EXISTS (SELECT * FROM dbo.GPost_BlackList WITH (NOLOCK) WHERE MyUID = @iMyUID AND BlockUID = @iBlockUID)
	BEGIN
		SELECT @iOK = -2
		GOTO END_PROC
	END

BEGIN TRAN
	DELETE FROM dbo.GPost_BlackList
		WHERE MyUID = @iMyUID AND BlockUID = @iBlockUID

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN
			SELECT @iOK = -3
			GOTO FAIL_TRAN
		END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


