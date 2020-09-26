CREATE PROCEDURE [dbo].[gup_update_read_post_letter]
	@UnitUID	bigint	
,	@iPostNo	bigint
,	@iOK		int = 0
AS
SET NOCOUNT ON;

DECLARE	@iToUnitUID	bigint
,		@iIsRead	bigint
,		@bDeleted	bit

SELECT @iToUnitUID = ToUnitUID, @iIsRead = IsRead, @bDeleted = Deleted
	FROM  dbo.GPost WITH (NOLOCK) WHERE PostNo = @iPostNo


	-- 해당 우체국번호가 없으면 에러
	IF @iToUnitUID IS NULL
		BEGIN	SELECT	@iOK = -2	GOTO END_PROC	END

	-- 이미 삭제된 아이템이면 에러
	IF (@bDeleted = 1)
		BEGIN	SELECT	@iOK = -3	GOTO END_PROC	END
	
	-- 다른 사람의 소포이면
	IF (@UnitUID <> @iToUnitUID)
		BEGIN	SELECT	@iOK = -4	GOTO END_PROC	END

	-- 이미 읽은 편지라면
	IF (@iIsRead = 1)
		BEGIN	SELECT	@iOK = -5	GOTO END_PROC	END


BEGIN TRAN
	
	UPDATE dbo.GPost
		SET IsRead = 1
			WHERE PostNo = @iPostNo
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT	@iOK = -1	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


