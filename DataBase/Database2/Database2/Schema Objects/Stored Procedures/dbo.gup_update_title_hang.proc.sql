/*	sutamtwo
	2008-10-07
	타이틀 장/탈착 
	넘어오는 타이틀 ID 값이 0 이면 탈착,
	0 이상이면 장착
*/

create PROCEDURE [dbo].[gup_update_title_hang]
(
	@iUnitUID	bigint
,	@iOldID		smallint
,	@iTitleID	smallint
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;


BEGIN TRAN

	-- 탈착하기 (@iTitleID = 0)
	IF	(@iTitleID = 0)
		BEGIN
			UPDATE dbo.GTitle_Complete
				SET IsHang = 0
					WHERE UnitUID = @iUnitUID
			IF @@ERROR <> 0 
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
		END

	-- 새로 장착하기 (@iTitleID > 0)
	ELSE
		BEGIN
		-- 기존꺼 빼고
			UPDATE dbo.GTitle_Complete
				SET IsHang = 0
					WHERE UnitUID = @iUnitUID AND TitleID = @iOldID
			IF @@ERROR <> 0 OR @@ROWCOUNT > 1
				BEGIN	SELECT @iOK = -3	GOTO FAIL_TRAN	END

		-- 새로 넣고
			UPDATE dbo.GTitle_Complete
				SET IsHang = 1
					WHERE UnitUID = @iUnitUID AND TitleID = @iTitleID
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END

		END
	
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


