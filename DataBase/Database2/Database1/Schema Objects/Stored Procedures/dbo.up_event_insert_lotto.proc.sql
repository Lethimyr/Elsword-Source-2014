CREATE PROCEDURE [dbo].[up_event_insert_lotto]
	@strLogin_ [nvarchar](20),
	@iCount_ [int] = 1
AS
exec dbo.up_log_set_proc_count 'up_event_insert_lotto'

SET NOCOUNT ON

DECLARE @iOK int

--퀴니 로또 이벤트 아이템 종료로 인한 프로시저 변경

--IF GetDate() < '2005-10-08'
--BEGIN
    SET @iOK = -1
    GOTO END_PROC
--END

IF NOT EXISTS( SELECT * FROM dbo.Users(NOLOCK) WHERE login = @strLogin_ )
BEGIN
	SET @iOK = -1
	GOTO END_PROC
END

IF EXISTS( SELECT * FROM dbo.EventLottoU(NOLOCK) WHERE login = @strLogin_ )
    BEGIN
    	DECLARE @iCount int
        DECLARE @RegDate smalldatetime
    	SELECT @iCount = [Count], @RegDate = RegDate FROM dbo.EventLottoU(NOLOCK) WHERE login = @strLogin_

        IF YEAR(@RegDate) = YEAR(GetDate()) AND MONTH(@RegDate) = MONTH(GetDate()) AND DAY(@RegDate) = DAY(GetDate())
            BEGIN
        		SET @iOK = -2
                GOTO END_PROC
            END

    	SET @iCount = @iCount + @iCount_
    	IF @iCount < 0
        	BEGIN
        		SET @iCount = 0
        	END

        BEGIN TRAN
        UPDATE dbo.EventLottoU WITH (ROWLOCK) SET [Count] = @iCount, RegDate = GetDate() WHERE login = @strLogin_
        IF @@ERROR <> 0    BEGIN    GOTO Failed    END
    END
ELSE
    BEGIN
    	IF @iCount_ < 0
        	BEGIN
        		SET @iCount_ = 0
        	END
    
        BEGIN TRAN
        INSERT INTO dbo.EventLottoU(login, [Count], RegDate) VALUES(@strLogin_, @iCount_, GetDate())
        IF @@ERROR <> 0    BEGIN    GOTO Failed    END
    END

COMMIT TRAN
SET @iOK = 0
goto END_PROC

Failed:
ROLLBACK TRAN
SET @iOK = -3 --이쪽 루트로 지나가는 일은 최대한 막아보세 -_-;;;
END_PROC:
SELECT @iOK
RETURN


