CREATE PROCEDURE [dbo].[gup_pointbag_recommend]
	@iLoginUID_ [int],
	@strNick_ [nvarchar](24),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 12일
사용 : 신규유저의 기존유저 추천을 기록해준다.
*/
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.GPointBag WITH (NOLOCK) WHERE LoginUID = @iLoginUID_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @iRecommendeeLoginUID int
SELECT  @iRecommendeeLoginUID = a.LoginUID
    FROM dbo.Users as a WITH (NOLOCK)
        WHERE EXISTS(SELECT * FROM dbo.NickNames as b WITH (NOLOCK) WHERE a.Login = b.Login AND b.Nick = @strNick_)

IF @iRecommendeeLoginUID IS NULL
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @iLoginUID_ = @iRecommendeeLoginUID
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

BEGIN TRAN
    exec @iOK = dbo.gup_pointbag_get @iRecommendeeLoginUID, 1
    if @iOK <> 0 --만약 추천받은 유저가 아직 포인트 기록이 없다면...
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    UPDATE dbo.GPointBag WITH (UPDLOCK)
        SET Point = Point + 700
            WHERE LoginUID = @iRecommendeeLoginUID
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END

    INSERT INTO dbo.GPointBagRecommend
            (
                LoginUID
,               RLoginUID
            )
        SELECT  @iRecommendeeLoginUID
,               @iLoginUID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


