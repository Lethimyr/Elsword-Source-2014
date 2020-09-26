CREATE PROCEDURE [dbo].[up_LimitGamePlay_Offline]
	@iLoginUID_ [int],
	@iPlayTime_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

BEGIN TRAN
    UPDATE dbo.LimitGamePlay WITH(UPDLOCK)
        SET OnlineTime = OnlineTime + @iPlayTime_
,           LastLogin = GetDate()
            WHERE LoginUID = @iLoginUID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN
            ROLLBACK TRAN
            SELECT @iOK = -11
            GOTO END_PROC
        END
COMMIT TRAN

END_PROC:

SELECT @iOK


