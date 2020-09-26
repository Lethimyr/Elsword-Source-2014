CREATE PROCEDURE [dbo].[up_delete_guild]
	@iGuildId_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_delete_guild'

SET NOCOUNT ON

-- 해당 길드가 존재 하는가?
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId_ )
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

BEGIN TRAN
    -- 각 길드원에 대한 탈퇴 처리
    DELETE FROM dbo.GuildUser WHERE GuildId = @iGuildId_
    IF @@ERROR <> 0
        GOTO lbTran_Abort
    -- 길드 관련 제거
    DELETE FROM dbo.GuildInfo WHERE GuildId = @iGuildId_
    IF @@ERROR <> 0
        GOTO lbTran_Abort
    -- Notice 제거
    DELETE FROM dbo.Notice WHERE GuildId = @iGuildId_
    IF @@ERROR <> 0
        GOTO lbTran_Abort    

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SET @iOK_ = -9
RETURN @iOK_

lbTran_Success:
SET @iOK_ = 0
RETURN @iOK_


