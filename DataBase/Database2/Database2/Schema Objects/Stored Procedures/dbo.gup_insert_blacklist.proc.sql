/*	sutamtwo
	2008-01-31
	블랙 리스트에 등재하기
*/

CREATE PROCEDURE [dbo].[gup_insert_blacklist]
(
	@iMyUID		bigint	
,	@iBlockUID	bigint	
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;

DECLARE @strNickName nvarchar(16)
SELECT	@strNickName = Nickname 
	FROM dbo.GUnitNickName WITH (NOLOCK)
		WHERE UnitUID = @iBlockUID

IF (@strNickName IS NULL)
	BEGIN	SELECT @iOK = -1	GOTO END_PROC	END

BEGIN TRAN
	INSERT INTO dbo.GBlackList(MyUID, BlockUID)
		SELECT @iMyUID, @iBlockUID
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN


END_PROC:
SELECT @iOK, @strNickName


