CREATE PROCEDURE [dbo].[up_be_in_use_guild_name]
	@strName_ [nvarchar](12),
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_be_in_use_guild_name'

SET NOCOUNT ON

-- 요청한 길드의 존재, 승인 여부 확인
IF EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE Name = @strName_)
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

SET @iOK_ = 0
RETURN @iOK_


