CREATE PROCEDURE [dbo].[up_LimitGamePlay_Online]
	@iLoginUID_ [int],
	@iThresholdTime_ [int],
	@iFullTime_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @iOnlineTime int
,       @iOfflineTime int
,       @sdtNow smalldatetime

SELECT @sdtNow = GetDate()

SELECT  @iOnlineTime = OnlineTime
,       @iOfflineTime = OfflineTime + CASE WHEN DateDiff(mi, LastLogin, @sdtNow) >= @iThresholdTime_ THEN DateDiff(mi, LastLogin, @sdtNow) ELSE 0 END
    FROM dbo.LimitGamePlay(NOLOCK)
        WHERE LoginUID = @iLoginUID_

IF @iOnlineTime is NULL OR @iOffLineTime is NULL
    BEGIN
        SELECT  @iOnlineTime = 0
,               @iOfflineTime = 0
        IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iLoginUID_)
            BEGIN    SELECT @iOK = -1    GOTO END_PROC    END
        BEGIN TRAN
            INSERT INTO dbo.LimitGamePlay(LoginUID, OnlineTime, OfflineTime, LastLogin)
                SELECT @iLoginUID_, @iOnlineTime, @iOfflineTime, @sdtNow
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN
                    ROLLBACK TRAN
                    SELECT @iOK = -11
                    GOTO END_PROC
                END
        COMMIT TRAN
    END
ELSE
    BEGIN
        IF @iOffLineTime >= @iFullTime_
            BEGIN
                SELECT  @iOnlineTime = 0
,                       @iOffLineTime = 0
            END
        BEGIN TRAN
            UPDATE dbo.LimitGamePlay WITH (ROWLOCK)
                SET OnlineTime = @iOnlineTime
,                   OfflineTime = @iOfflineTime
,                   LastLogin = @sdtNow
                    WHERE LoginUID = @iLoginUID_
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN
                    ROLLBACK TRAN
                    SELECT @iOK = -12
                    GOTO END_PROC
                END
       COMMIT TRAN
    END

END_PROC:

SELECT @iOK, @iOnlineTime, @iOfflineTime


