CREATE PROCEDURE [dbo].[up_recommender_update]
	@iRecommenderUID_ [int],
	@iPoint_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @strLogin nvarchar(20)
SELECT @strLogin = [Login]
    FROM dbo.Users(NOLOCK)
        WHERE LoginUID = @iRecommenderUID_
IF @strLogin is NULL
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @iRecommendeeUID int
SELECT @iRecommendeeUID = -1
SELECT @iRecommendeeUID = RecommendeeUID
    FROM dbo.RecommenderList(NOLOCK)
        WHERE RecommenderUID = @iRecommenderUID_
IF @iRecommendeeUID = -1
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iRecommendeeUID)
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.RecommenderPointList(NOLOCK) WHERE LoginUID = @iRecommendeeUID)
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

IF @iPoint_ < 10
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

DECLARE @iLimit int
SELECT @iLimit = 120
SELECT @iLimit = Variable1
    FROM dbo.GlobalVariables(NOLOCK)
        WHERE [No] = 6

IF @iPoint_ > @iLimit
    BEGIN    SELECT @iPoint_ = @iLimit    END

DECLARE @iLevelTotal int
SELECT @iLevelTotal = 0
SELECT @iLevelTotal = @iLevelTotal + SUM([Level])
    FROM dbo.Characters(NOLOCK)
        WHERE [Login] = @strLogin

SELECT @iPoint_ = @iLevelTotal * @iPoint_ / 10

BEGIN TRAN
    UPDATE dbo.RecommenderList WITH(UPDLOCK)
        SET Point = Point + @iPoint_
            WHERE RecommenderUID = @iRecommenderUID_ AND RecommendeeUID = @iRecommendeeUID
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    UPDATE dbo.RecommenderPointList WITH(UPDLOCK)
        SET Point = Point + @iPoint_
            WHERE LoginUID = @iRecommendeeUID
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


