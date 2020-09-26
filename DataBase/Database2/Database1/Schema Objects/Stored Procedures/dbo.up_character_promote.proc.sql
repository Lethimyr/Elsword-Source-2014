CREATE PROCEDURE [dbo].[up_character_promote]
	@strLogin_ [nvarchar](20),
	@iCharType_ [tinyint],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

-- 존재하는 계정인지 확인
IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

-- 해당 캐릭터를 소유하고 있는지 확인
IF NOT EXISTS(SELECT * FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = @iCharType_)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

-- 전직이 가능한 단계인지 확인
DECLARE @iPromotionLevel int
SET @iPromotionLevel = -1
SELECT @iPromotionLevel = Promotion FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = @iCharType_

IF @iPromotionLevel = -1 -- 현재의 전직상태가 올바르지 않음.
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

SELECT @iPromotionLevel = @iPromotionLevel + 1;

IF NOT @iPromotionLevel BETWEEN 1 AND 3  -- 060516. 현재 2차전직까지 열림. --070704. 3차 전직까지 가능하도록 오픈
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

BEGIN TRAN
    UPDATE dbo.Characters WITH (ROWLOCK)
        SET Promotion = @iPromotionLevel
            WHERE Login = @strLogin_ AND CharType = @iCharType_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -5    GOTO FAIL_TRAN    END
COMMIT TRAN

SELECT @iOK, @iPromotionLevel
RETURN

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


