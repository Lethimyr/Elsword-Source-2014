/*	sutamtwo
	2008-10-07
	미션 성공 -> 미션 지우기 -> 칭호 받기
	PERIOD 값이 0이면 영구, 나머지 숫자면 더해서 종료일 계산
*/

create PROCEDURE [dbo].[gup_insert_title]
(
	@iUnitUID	bigint
,	@iTitleID	smallint
,	@iPeriod	smallint
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;

DECLARE	@sdtEndDate	smalldatetime
,		@iIsExist	tinyint

IF @iPeriod = 0
	BEGIN	
		SELECT	@sdtEndDate = '2040-12-31 23:59'	
	END

ELSE
	BEGIN
		SELECT	@sdtEndDate = DATEADD(DD, @iPeriod, GETDATE())
	END



SELECT @iIsExist = (SELECT COUNT(*) FROM dbo.GTitle_Complete WITH (NOLOCK) WHERE UnitUID = @iUnitUID AND TitleID = @iTitleID)



BEGIN TRAN

	-- 처음 들어온 Title이면
	IF	(@iIsExist = 0)
		BEGIN
			INSERT INTO dbo.GTitle_Complete (UnitUID, TitleID, EndDate, IsHang)
				SELECT @iUnitUID, @iTitleID, @sdtEndDate, 0
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
		
		END
	-- 이미 들어왔던 기록이 있으면
	ELSE
		BEGIN
			UPDATE dbo.GTitle_Complete
				SET EndDate = @sdtEndDate
					WHERE UnitUID = @iUnitUID AND TitleID = @iTitleID
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END

		END

	-- 진행 중이던 미션 지우기
	DELETE FROM dbo.GTitle_Mission
		WHERE UnitUID = @iUnitUID AND TitleID = @iTitleID
	--IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
	--	BEGIN	SELECT @iOK = -3	GOTO FAIL_TRAN	END

	
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @sdtEndDate


