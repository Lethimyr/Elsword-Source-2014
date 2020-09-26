CREATE PROCEDURE [dbo].[up_recommender_insert]
	@iRecommenderUID_ [int],
	@iRecommendeeUID_ [int] = -1,
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iRecommenderUID_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF EXISTS(SELECT * FROM dbo.RecommenderList(NOLOCK) WHERE RecommenderUID = @iRecommenderUID_)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @iRecommendeeUID_ <> -1 AND NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iRecommendeeUID_)
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

DECLARE @bExist bit
SELECT @bExist = 0
IF EXISTS(SELECT * FROM dbo.RecommenderPointList(NOLOCK) WHERE LoginUID = @iRecommendeeUID_)
    BEGIN    SELECT @bExist = 1    END

BEGIN TRAN
    INSERT INTO dbo.RecommenderList(RecommenderUID, RecommendeeUID)
        SELECT @iRecommenderUID_, @iRecommendeeUID_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    INSERT INTO dbo.RecommenderPointList(LoginUID, Point)
        SELECT @iRecommenderUID_, 500
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END

    IF @bExist = 0 AND @iRecommendeeUID_ <> -1
        BEGIN
            INSERT INTO dbo.RecommenderPointList(LoginUID)
                SELECT @iRecommendeeUID_
            IF @@ERROR <> 0
                BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END
        END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


