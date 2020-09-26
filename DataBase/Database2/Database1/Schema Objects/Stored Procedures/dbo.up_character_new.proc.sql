CREATE PROCEDURE [dbo].[up_character_new]
	@strLogin_ [nvarchar](20),
	@iCharType_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @iCharType tinyint
,       @iPromotion tinyint
,       @iExp bigint
,       @iWin int
,       @iLose int

SELECT  @iCharType = 255
,       @iPromotion = 255
,       @iExp = 0
,       @iWin = -1
,       @iLose = -1


-- 계정 존재 여부 확인
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strLogin_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

-- 캐릭터 존재 여부 확인
IF EXISTS(SELECT * FROM dbo.characters(NOLOCK) WHERE login = @strLogin_ AND chartype = @iCharType_)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

BEGIN TRAN
    -- 실제 캐릭터 정보 추가
    INSERT INTO dbo.characters(login, chartype) SELECT @strLogin_, @iCharType_
    IF @@ERROR <> 0
        BEGIN    SET @iOK = -11    GOTO TRAN_FAIL    END
COMMIT TRAN

SELECT  @iCharType = CharType
,       @iPromotion = Promotion
,       @iExp = [Exp]
,       @iWin = Win
,       @iLose = Lose
    FROM dbo.characters(NOLOCK)
        WHERE login = @strLogin_ AND chartype = @iCharType_

GOTO END_PROC

TRAN_FAIL:
ROLLBACK TRAN

END_PROC:
SELECT  @iOK
,       @iCharType
,       @iPromotion
,       @iExp
,       @iWin
,       @iLose


