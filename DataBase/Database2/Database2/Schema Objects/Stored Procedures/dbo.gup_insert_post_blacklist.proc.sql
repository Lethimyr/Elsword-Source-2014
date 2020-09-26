/*	sutamtwo
	2008-09-26
	블랙 리스트에 등재하기

	-1 : 해당 닉네임 없음
	-2 : 이미 등록된 이름
*/

create PROCEDURE [dbo].[gup_insert_post_blacklist]
(
	@iMyUID		bigint	
,	@iBlockNick	nvarchar(16)
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;

DECLARE @iBlockUID	bigint
SELECT	@iBlockUID = UnitUID
	FROM dbo.GUnitNickName WITH (NOLOCK)
		WHERE NickName = @iBlockNick

-- 해당 닉네임은 없다
IF (@iBlockUID IS NULL)
	BEGIN	SELECT @iOK = -1	GOTO END_PROC	END

-- 이미 등록된 닉네임이다
IF EXISTS (SELECT * FROM dbo.GPost_BlackList WITH (NOLOCK) WHERE MyUID = @iMyUID AND BlockUID = @iBlockUID)
	BEGIN	SELECT @iOK = -2	GOTO END_PROC	END



BEGIN TRAN
	INSERT INTO dbo.GPost_BlackList (MyUID, BlockUID)
		SELECT @iMyUID, @iBlockUID
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -3	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN


END_PROC:
SELECT @iOK


