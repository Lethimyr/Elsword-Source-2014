/*
	sutamtwo
	부활석 사용 후 남은 갯수(변동량)을 DB에 저장
	2007. 11. 20

	OK값
	
	 -1	: 해당 유닛 없음
	-10 : 부활석이 없을 때 Insert 오류 ( 처음 Insert 하는데 그 값이 음수면 곤란함)
	-20 : 계산값이 음수일 때 0으로 바꾸고 업데이트 하는 행
	-21 : 보통 일반적인 업데이트문
*/
CREATE PROCEDURE [dbo].[gup_update_resurrection_stone]
    @iUnitUID   	[bigint]
,	@iResurrection	[int]
,	@iOK        	[int] = 0
AS

SET NOCOUNT ON;
SET XACT_ABORT ON;

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END


DECLARE @iStoneCnt	int
SELECT @iStoneCnt = Quantity
	FROM dbo.GResurrectionStone	WITH (NOLOCK)
		WHERE UnitUid = @iUnitUID


BEGIN TRAN

	-- 부활석 존재하지 않으면 Insert
	IF NOT EXISTS (SELECT * FROM dbo.GResurrectionStone WITH (NOLOCK) WHERE UnitUID = @iUnitUID)
		BEGIN
			INSERT INTO dbo.GResurrectionStone (UnitUID, Quantity)
				SELECT @iUnitUID, @iResurrection
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -10 GOTO END_PROC	END

			GOTO COMMIT_TRAN
		END
		
	
	-- 계산 값이 음수가 될 경우 0으로 저장
	IF ((@iStoneCnt + @iResurrection)<0)
	BEGIN
		UPDATE dbo.GResurrectionStone WITH (UPDLOCK)
			SET	Quantity = 0
				WHERE unitUID = @iUnitUID
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN 	SELECT @iOK = -20	GOTO END_PROC	END

		GOTO COMMIT_TRAN
	END
	
	UPDATE dbo.GResurrectionStone WITH (UPDLOCK)
		SET	Quantity = @iStoneCnt + @iResurrection
			WHERE unitUID = @iUnitUID
		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
			BEGIN 	SELECT @iOK = -21	GOTO END_PROC	END

COMMIT_TRAN:
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


