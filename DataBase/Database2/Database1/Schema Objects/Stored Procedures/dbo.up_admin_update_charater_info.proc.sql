CREATE PROCEDURE [dbo].[up_admin_update_charater_info]
	@strLogin_ [nvarchar](20),
	@iCharType_ [int],
	@iExp_ [int]
AS
exec dbo.up_log_set_proc_count 'up_admin_update_character_info'

SET NOCOUNT ON

DECLARE @iExp           bigint,
        @iAllExp        bigint,
        @iNumCharacters int

-- 해당 사용자가 존재 하는지 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strLogin_)
BEGIN
    SELECT -1
    RETURN
END

IF NOT EXISTS(SELECT * FROM dbo.characters(NOLOCK) WHERE login = @strLogin_ AND chartype = @iCharType_ ) 
BEGIN
    SELECT -2
    RETURN
END
SELECT @iExp = Exp FROM dbo.characters(NOLOCK) WHERE login = @strLogin_ AND chartype = @iCharType_


BEGIN TRAN

-- 주어진 경치가 0이면 대상 사용자의 정보를 초기화 함
IF @iExp_ = 0 
BEGIN
   UPDATE dbo.characters SET Exp = 100 WHERE login = @strLogin_ and chartype = @iCharType_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

--모든 캐릭의 경험치가 모두 0이면 대상 사용자의 정보를 초기화 함
   SELECT    @iAllExp = SUM(Exp), @iNumCharacters = count(*) FROM dbo.characters(NOLOCK) where login = @strLogin_

   IF @iAllExp = 100 * @iNumCharacters
   BEGIN
       UPDATE dbo.users SET gamePoint = 200 WHERE login = @strLogin_
       IF @@ERROR <> 0
         GOTO lbTran_Abort
   END 

    GOTO lbTran_Success

END

-- 주어진 파라미터 값의 범위를 확인, 합법적인 값이면 변경(이상한 값이면 값을 변경 시키지 않음)
IF @iExp_ >= 100
BEGIN
    SET @iExp = @iExp_
END
ELSE
BEGIN
    SELECT -3
    RETURN
END

-- 해당 사용자의 정보를 갱신 한다.
UPDATE dbo.characters SET Exp = @iExp WHERE login = @strLogin_ AND chartype = @iCharType_
        IF @@ERROR <> 0
                     GOTO lbTran_Abort
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9
RETURN

lbTran_Success:
COMMIT TRAN
SELECT 0
RETURN


