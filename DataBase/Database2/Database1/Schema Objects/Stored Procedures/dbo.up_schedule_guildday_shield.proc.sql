CREATE PROCEDURE [dbo].[up_schedule_guildday_shield]
AS
SET NOCOUNT ON
SET XACT_ABORT ON

DECLARE @iTopRankGuildID	int, 
        @iSecondRankGuildID	int,
   	    @iThirdRankGuildID	int,
        @iTopRankShieldGoodsID	int, 
        @iSecondRankShieldGoodsID	int, 
        @iThirdRankShieldGoodsID	int, 
        @iGuildShieldUID	int,
        @iShieldGoodsID int,
        @iGuildID	int,
        @strUserLogin	nvarchar(12)

-- 길드방패 하드코딩
SET @iTopRankShieldGoodsID     = 11500
SET @iSecondRankShieldGoodsID  = 11510
SET @iThirdRankShieldGoodsID   = 11520

CREATE TABLE #t (
Ranking int IDENTITY( 1, 1) NOT NULL, 
GuildID	int  NOT NULL, 
Point	int  NOT NULL )

SET NOCOUNT ON
-- 현재 가지고 있는 길드방패를 모두 제거한다.
DECLARE SetDelGuildShield_Cursor CURSOR FORWARD_ONLY FOR
SELECT GuildShieldUID FROM GuildShieldUser(NOLOCK)
OPEN SetDelGuildShield_Cursor
FETCH NEXT FROM SetDelGuildShield_Cursor INTO @iGuildShieldUID
WHILE @@FETCH_STATUS = 0
BEGIN
   IF EXISTS( SELECT * FROM GoodsObjectlist(NOLOCK)  WHERE ItemUID = @iGuildShieldUID)
   BEGIN
      SELECT @iShieldGoodsID = ItemID FROM GoodsObjectlist(NOLOCK)  WHERE ItemUID = @iGuildShieldUID
      IF  @iShieldGoodsID = @iTopRankShieldGoodsID OR @iShieldGoodsID = @iSecondRankShieldGoodsID OR @iShieldGoodsID = @iThirdRankShieldGoodsID
      BEGIN
          DELETE FROM GoodsObjectList WHERE ItemUID = @iGuildShieldUID
      END
   END
FETCH NEXT FROM SetDelGuildShield_Cursor INTO @iGuildShieldUID
END
CLOSE SetDelGuildShield_Cursor
DEALLOCATE SetDelGuildShield_Cursor

--이전의 목록은 다 지운다.
truncate table GuildShieldUser

--1위부터3위까지  guildid를 얻어온다.
INSERT INTO #t ( GuildID, Point ) SELECT TOP 3 GuildID, Point FROM ranking_guildday(NOLOCK)  ORDER BY Point DESC 
SELECT @iTopRankGuildID = GuildID FROM #t WHERE Ranking = 1
SELECT @iSecondRankGuildID = GuildID FROM #t WHERE Ranking = 2
SELECT @iThirdRankGuildID = GuildID FROM #t WHERE Ranking = 3

--길드 방패 넣어주기 
DECLARE @sdtNow smalldatetime
SET @sdtNow = GetDate()
DECLARE SetTopGuild_Cursor CURSOR FORWARD_ONLY FOR
SELECT Login, GuildID FROM GuildUser(NOLOCK) WHERE GuildID = @iTopRankGuildID OR GuildID = @iSecondRankGuildID OR GuildID = @iThirdRankGuildID
OPEN SetTopGuild_Cursor
FETCH NEXT FROM SetTopGuild_Cursor INTO @strUserLogin, @iGuildID
WHILE @@FETCH_STATUS = 0
BEGIN
    IF  @iGuildID = @iTopRankGuildID --1위
    BEGIN
      SET @iShieldGoodsID = @iTopRankShieldGoodsID
    END
    ELSE IF @iGuildID = @iSecondRankGuildID --2위
    BEGIN
      SET @iShieldGoodsID = @iSecondRankShieldGoodsID
    END
    ELSE  --3위
    BEGIN
      SET @iShieldGoodsID = @iThirdRankShieldGoodsID
    END
    --방패 넣기 
    INSERT INTO GoodsObjectList( OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period )
        VALUES ( @strUserLogin, 'GuildShield',  @iShieldGoodsID, @sdtNow, @sdtNow, DateAdd(dd, -1, @sdtNow), -1 )
    --UID구하기
    SET @iGuildShieldUID = SCOPE_IDENTITY()
   --나중에 한달전에 받은 유저를 지우기 위해서 기억해둠
    INSERT INTO GuildShieldUser( Login, GuildShieldUID, ShieldGoodsID, GuildID ) 
       VALUES ( @strUserLogin, @iGuildShieldUID, @iShieldGoodsID, @iGuildID )

FETCH NEXT FROM SetTopGuild_Cursor INTO @strUserLogin, @iGuildID
END


