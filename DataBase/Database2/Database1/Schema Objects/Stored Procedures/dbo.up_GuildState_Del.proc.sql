CREATE PROCEDURE [dbo].[up_GuildState_Del]
	@strReqLogin_ [nvarchar](20),
	@strLogin_ [nvarchar](20),
	@iGuildId_ [int],
	@Curl_ [nvarchar](20),
	@nNO_ [nvarchar](50),
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_GuildState_Del'

SET NOCOUNT ON

DECLARE @UserID_   nvarchar(20),
        @Str_      nvarchar(100)    

-- 요청자가 길마인지 확인
--IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildId_ AND UserLevel >= 3 )
IF @strReqLogin_ <> @strLogin_ AND
   NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildId_ AND UserLevel >= 3 )
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

IF NOT EXISTS(SELECT [ID] From dbo.GuildUserState(nolock) WHERE Curl=@Curl_)
Begin
    Set @iOK_ = -2
    RETURN @iOK_End

-- 게임머니 지급이 안된 추천자를 찾는 커서 시작 
    DECLARE DeleteUser_Cursor CURSOR FOR

    SELECT [ID] FROM dbo.GuildUserState (nolock) WHERE Curl = @Curl_ AND [NO] in (Convert(int,@nNo_))
    

    OPEN DeleteUser_Cursor
    
    FETCH NEXT FROM DeleteUser_Cursor
    INTO @UserID_
    
    WHILE (@@FETCH_STATUS = 0)
    BEGIN
                
        DELETE GuildUserState WHERE [ID] = @UserID_
                                
        FETCH NEXT FROM DeleteUser_Cursor
        INTO @UserID_
        
    END
    
    CLOSE DeleteUser_Cursor
    DEALLOCATE DeleteUser_Cursor

    SET @iOK_ = 0
    RETURN @iOK_


