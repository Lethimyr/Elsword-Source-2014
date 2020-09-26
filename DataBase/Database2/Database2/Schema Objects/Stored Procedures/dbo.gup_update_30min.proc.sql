CREATE PROCEDURE [dbo].[gup_update_30min]
	@iUnitUID	BIGINT
,	@iEventType	smallint
,	@sdtEndDate	smalldatetime
,	@iOK		INT = 0
AS 

/* 
sutamtwo
작성일 : 2008년 05월 19일
사용 : 30분 접속 이벤트 (UPDATE 용)
*/
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN
	IF EXISTS (SELECT * FROM dbo.GIs30Min WITH (NOLOCK) WHERE UnitUID = @iUnitUID AND EventType = @iEventType)
		BEGIN
			UPDATE dbo.GIs30Min WITH (UPDLOCK)
				SET	RegDate = @sdtEndDate
					WHERE UnitUID = @iUnitUID AND EventType = @iEventType
			IF @@ERROR <> 0 OR @@ROWCOUNT > 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
		END
	ELSE 
		BEGIN
			INSERT INTO dbo.GIs30Min (UnitUID, EventType, RegDate)
				SELECT @iUnitUID, @iEventType, @sdtEndDate
			IF @@ERROR <> 0 OR @@ROWCOUNT > 1
				BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END
		END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:

SELECT @iOK


