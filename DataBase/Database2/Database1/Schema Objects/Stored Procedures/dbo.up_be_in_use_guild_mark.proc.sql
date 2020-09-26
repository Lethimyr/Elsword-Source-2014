CREATE PROCEDURE [dbo].[up_be_in_use_guild_mark]
	@iMark_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_be_in_use_guild_mark'

SET NOCOUNT ON

-- 요청한 길드의 존재, 승인 여부 확인
DECLARE @iMark int
EXEC @iMark = dbo.up_mark2int @iMark_

IF EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE Mark = @iMark)
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

SET @iOK_ = 0
RETURN @iOK_


