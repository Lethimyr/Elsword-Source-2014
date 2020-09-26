CREATE PROCEDURE [dbo].[up_remove_guild]
	@iGuildId [int],
	@strUserid [nvarchar](40),
	@iOK [int] = 1
AS
SET NOCOUNT ON;

DECLARE @strName nvarchar(24)
,       @iDelPoint int
,       @iMemberCnt int
,       @strCafeUrl varchar(20)

SELECT  @strName = Name
,       @iDelPoint = Point
,       @iMemberCnt = NumMembers
,       @strCafeUrl = cafe_url
    FROM dbo.GuildInfo WITH (NOLOCK)
        WHERE LoadLogin = @strUserid AND guildid = @iGuildId

IF @strName is NULL
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @iMemberCnt <> 1
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

BEGIN TRAN
    INSERT INTO dbo.DelGuildInfo
            (
                GuildName
,               Point
,               RegDate
            )
        SELECT  @strName
,               @iDelPoint
,               GetDate()
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END

    DELETE FROM dbo.GuildUserState
        WHERE GuildId = @iGuildId
    IF @@ERROR <> 0
    BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END

    DELETE FROM dbo.GuildInfo
        WHERE GuildId = @iGuildId AND Name = @strName
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END

    DELETE FROM dbo.GuildUser
        WHERE GuildId = @iGuildId
    IF @@ERROR <> 0
    BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END

    DELETE FROM dbo.Notice
        WHERE GuildId = @iGuildId
    IF @@ERROR <> 0
    BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
RETURN @iOK

--/***************************************************
--@author		: 남주희
--@date		: 2007-04-06
--@modified	: 
--@desc		: 길드마스터에 의한 길드해체
--
--Exec dbo.up_remove_guild 회원아이디, 길드번호
--Exec dbo.up_remove_guild 'nm_leader',4295
--***************************************************/
--
--CREATE PROCEDURE [dbo].[up_remove_guild]
--(
--	@iGuildId       int,
--	@strUserid      nvarchar(40)
--)
--AS
--
--
--SET NOCOUNT ON
--
--DECLARE @strName        nvarchar(24), 
--        @iDelPoint      int,
--		@iMemberCnt	    int,
--		@strCafeUrl     varchar(20)
--BEGIN
--    IF EXISTS( SELECT count(guildid) FROM dbo.GuildInfo(NOLOCK) WHERE LoadLogin = @strUserid And guildid = @iGuildId)
--    
--    BEGIN
--    	--1.길드이름, 길드포인트 길드원수,길드URL 정보 
--    	SELECT @strName=Name, @iDelPoint=Point, @iMemberCnt=NumMembers, @strCafeUrl=cafe_url FROM dbo.GuildInfo(NOLOCK) WHERE guildid = @iGuildId
--
--		IF @iMemberCnt > 1 
--		BEGIN
--			RETURN -3
--		END		
--		
--		Else IF @iMemberCnt = 1 
--		BEGIN
--			BEGIN TRAN
--				INSERT INTO dbo.DelGuildInfo ( GuildName, Point, RegDate ) VALUES ( @strName, @iDelPoint, GETDATE())
--				
--				DELETE FROM dbo.GuildUserState WHERE GuildId = @iGuildId
--				DELETE FROM dbo.Guildinfo WHERE GuildId = @iGuildId AND Name = @strName
--				DELETE FROM dbo.GuildUser WHERE GuildId = @iGuildId
--				DELETE FROM dbo.Notice  WHERE GuildId = @iGuildId
--				
--				IF @@ERROR <> 0 
--					GOTO ERRTRAP  
--		
--			COMMIT TRAN
--			RETURN 1		
--			
--	    
--			ERRTRAP:
--				ROLLBACK TRAN  
--				RETURN -1
--		END
--    END
--    Else
--    BEGIN
--    	
--		RETURN -2
--    END
--END


