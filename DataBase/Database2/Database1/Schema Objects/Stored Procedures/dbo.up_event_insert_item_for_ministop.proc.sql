CREATE PROCEDURE [dbo].[up_event_insert_item_for_ministop]
	@strLogin_ [nvarchar](20)
AS
/*
프로시저 설명
입력 : @strLogin_ nvarchar(20) --넷마블 ID를 입력하면 됩니다.
출력 : SELECT @iOK, RETURN @iOK --@iOK값으로 프로시저의 성공/실패 여부를 확인할 수 있습니다.
@iOK ( 0) : 성공/아이템이 정상적으로 지급되었음
     (-1) : 해당 유저가 존재하지 않음
     (-2이하) : 트랜잭션 작업 중 문제 발생

예제 : exec dbo.up_event_insert_item_for_ministop 'microcat'
*/

SET NOCOUNT ON;

DECLARE @iOK int, @iGoodsID int, @iDuration int, @bExists bit, @strMark nvarchar(20)
SELECT @iOK = 0
      ,@iGoodsID = 19360 --수호의 반지(10회)
      ,@iDuration = 10
      ,@bExists = 0
      ,@strMark = '_EventMiniStop_'

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN   SET @iOK = -1   GOTO END_PROC   END

IF EXISTS(SELECT * FROM dbo.DurationItemObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID)
    BEGIN   SET @bExists = 1    END

BEGIN TRAN
    IF @bExists = 0 --아이템이 없음
        BEGIN
            INSERT INTO dbo.DurationItemObjectList(OwnerLogin, BuyerLogin, GoodsID, Duration)
                VALUES(@strLogin_, @strMark, @iGoodsID, @iDuration)
            IF @@ERROR <> 0 BEGIN   SET @iOK = -2   GOTO FAIL_TRAN  END
        END
    ELSE
        BEGIN --아이템이 있음
            UPDATE dbo.DurationItemObjectList WITH (UPDLOCK)
                SET Duration = Duration + @iDuration
                    WHERE OwnerLogin = @strLogin_ AND GoodsID = @iGoodsID
            IF @@ERROR <> 0 BEGIN   SET @iOK = -3   GOTO FAIL_TRAN  END
        END

COMMIT TRAN
GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK
RETURN @iOK


