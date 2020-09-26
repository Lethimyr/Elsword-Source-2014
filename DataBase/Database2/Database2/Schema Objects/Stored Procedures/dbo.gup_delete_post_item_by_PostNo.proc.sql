CREATE PROCEDURE [dbo].[gup_delete_post_item_by_PostNo]
	@UnitUID	bigint	
,	@iPostNo	bigint
,	@iOK		int = 0
AS
SET NOCOUNT ON;

DECLARE @sdtNow	smalldatetime
SELECT	@sdtNow	= DATEADD(MI, 1, GETDATE())

	-- 해당 우체국번호가 없으면 에러
	IF NOT EXISTS (SELECT * FROM dbo.GPost WHERE PostNo = @iPostNo)
		BEGIN	SELECT	@iOK = -2	GOTO FAIL_TRAN	END

	-- 이미 삭제된 아이템이면 에러
	IF (1 = (SELECT DELETED FROM  dbo.GPost WHERE PostNo = @iPostNo))
		BEGIN	SELECT	@iOK = -3	GOTO FAIL_TRAN	END
	
	-- 다른 사람의 소포이면
	IF (@UnitUID <> (SELECT ToUnitUID FROM  dbo.GPost WHERE PostNo = @iPostNo))
		BEGIN	SELECT	@iOK = -4	GOTO FAIL_TRAN	END

BEGIN TRAN
	
	UPDATE dbo.GPost
		SET DELDATE = @sdtNow
			WHERE PostNo = @iPostNo
	IF @@ERROR <> 0
		BEGIN	SELECT	@iOK = -1	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


