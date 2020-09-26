CREATE PROCEDURE [dbo].[up_recommender_check_status]
	@iRecommenderUID_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @sdtFirstLogin smalldatetime
SELECT @sdtFirstLogin = 0
SELECT @sdtFirstLogin = FirstLogin
    FROM dbo.Users(NOLOCK)
        WHERE LoginUID = @iRecommenderUID_

IF @sdtFirstLogin = 0 --해당 유저가 존재하지 않음
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

IF @sdtFirstLogin < '2006-09-13' --해당 유저는 추천인 등록 대상자가 아님
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

DECLARE @iRecommendeeUID int
SELECT @iRecommendeeUID = -2
SELECT @iRecommendeeUID = RecommendeeUID
    FROM dbo.RecommenderList(NOLOCK)
        WHERE RecommenderUID = @iRecommenderUID_
IF @iRecommendeeUID = -2 --추천인을 등록해야됨(up_recommender_insert를 실행해서 추천인 등록 받을 것)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF @iRecommendeeUID = -1 --추천인을 지정하지 않고 등록했음
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END


DECLARE @iLimit int
SELECT @iLimit = 10
SELECT @iLimit = Variable1
    FROM dbo.GlobalVariables(NOLOCK)
        WHERE [No] = 5

IF DateAdd(dd, @iLimit, @sdtFirstLogin) < GetDate() --추천인 포인트 지급기간을 넘겼음
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

END_PROC:
SELECT @iOK


