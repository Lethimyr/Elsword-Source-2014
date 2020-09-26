CREATE PROCEDURE [dbo].[up_schedule_guildweek_shield]
AS
SET NOCOUNT ON
SET XACT_ABORT ON

DECLARE @iWeekRankGuildID    int, 
        @iWeekRankShieldGoodsID    int, 
        @iWeekGuildShieldUID    int,
        @iWeekShieldGoodsID int,
        @strUserLogin    nvarchar(12)

-- 길드방패 하드코딩
SET @iWeekRankShieldGoodsID     = 11530

SET NOCOUNT ON

-- 현재 가지고 있는 길드방패를 모두 제거한다.
DECLARE SetDelWeekShield_Cursor CURSOR FORWARD_ONLY FOR
SELECT GuildShieldUID FROM WeekShieldUser(NOLOCK)
OPEN SetDelWeekShield_Cursor
FETCH NEXT FROM SetDelWeekShield_Cursor INTO @iWeekGuildShieldUID
WHILE @@FETCH_STATUS = 0
BEGIN
   IF EXISTS( SELECT * FROM GoodsObjectlist(NOLOCK)  WHERE ItemUID = @iWeekGuildShieldUID)
   BEGIN
      SELECT @iWeekShieldGoodsID = ItemID FROM GoodsObjectlist(NOLOCK)  WHERE ItemUID = @iWeekGuildShieldUID
      IF  @iWeekShieldGoodsID = @iWeekRankShieldGoodsID
      BEGIN
          DELETE FROM GoodsObjectList WHERE ItemUID = @iWeekGuildShieldUID
      END
   END
FETCH NEXT FROM SetDelWeekShield_Cursor INTO @iWeekGuildShieldUID
END
CLOSE SetDelWeekShield_Cursor
DEALLOCATE SetDelWeekShield_Cursor

--이전의 목록은 다 지운다.
truncate table WeekShieldUser

--1위  guildid를 얻어온다.
SELECT TOP 1 @iWeekRankGuildID = GuildID FROM ranking_guildweek(NOLOCK) ORDER BY Point DESC

--길드 방패 넣어주기 
DECLARE @sdtNow smalldatetime
SET @sdtNow = GetDate()
DECLARE SetWeekGuild_Cursor CURSOR FORWARD_ONLY FOR
SELECT Login FROM GuildUser(NOLOCK) WHERE GuildID = @iWeekRankGuildID
OPEN SetWeekGuild_Cursor
FETCH NEXT FROM SetWeekGuild_Cursor INTO @strUserLogin
WHILE @@FETCH_STATUS = 0
BEGIN
    --방패 넣기 
    INSERT INTO GoodsObjectList( OwnerLogin, BuyerLogin, ItemID, RegDate, StartDate, EndDate, Period )
        VALUES ( @strUserLogin, 'GuildShield',  @iWeekRankShieldGoodsID, @sdtNow, @sdtNow, DateAdd(dd, -1, @sdtNow), -1 )
    --UID구하기
    -- SELECT @iWeekGuildShieldUID = MAX(GoodsUID) FROM GoodsObjectList
    SET @iWeekGuildShieldUID = SCOPE_IDENTITY()
   --나중에 일주일전에 받은 유저를 지우기 위해서 기억해둠
    INSERT INTO WeekShieldUser( Login, GuildShieldUID, ShieldGoodsID, GuildID ) 
       VALUES ( @strUserLogin, @iWeekGuildShieldUID, @iWeekRankShieldGoodsID, @iWeekRankGuildID )

FETCH NEXT FROM SetWeekGuild_Cursor INTO @strUserLogin
END
CLOSE SetWeekGuild_Cursor
DEALLOCATE SetWeekGuild_Cursor


