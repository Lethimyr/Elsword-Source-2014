CREATE PROCEDURE [dbo].[up_del_not_play_guild]
AS
exec dbo.up_log_set_proc_count 'up_del_not_play_guild'

SET NOCOUNT ON

DECLARE @strReNameSp    nvarchar(200), 
        @strName        nvarchar(12), 
        @iGuildId       int,
        @iTwoWeekPoint  int,
        @strCafeUrl     nvarchar(12)

set @strReNameSp = 'sp_rename ''ranking_guildweek_' + ltrim(rtrim(str(day(dateadd(day, -4, getdate()))))) + ''', ''temp_ranking'''
exec(@strReNameSp)

DECLARE Del_Guild_Cursor CURSOR FORWARD_ONLY FOR
        SELECT  GuildId, Name FROM dbo.ranking_guildweek(NOLOCK) WHERE Point = 0 --godsend 일주일 포인트가 0인 길드(변화량)를 찾아낸다
        OPEN Del_Guild_Cursor
        FETCH NEXT FROM Del_Guild_Cursor INTO @iGuildId, @strName--godsend  수정함 
        WHILE @@FETCH_STATUS = 0
        BEGIN
            IF EXISTS( SELECT * FROM dbo.temp_ranking(NOLOCK) WHERE GuildId = @iGuildId )
            BEGIN
              SELECT @iTwoWeekPoint = Point FROM dbo.temp_ranking WHERE GuildId = @iGuildId --그이전 일주일 포인트가 0인 길드는 삭제(10 일동안 길드변화량이 없다는 의미 ) 
            
--10일동안 포인트 변화률이 없다.
              IF @iTwoWeekpoint = 0
              BEGIN
--길드info테이블에 길드 id가 있으면 삭제..
                IF EXISTS(SELECT * FROM dbo.guildinfo(NOLOCK) WHERE GuildId = @iGuildId)
                BEGIN
                    --길드카페 주소를 얻어온다.
                    SELECT @strCafeUrl = cafe_url FROM dbo.guildinfo(NOLOCK) WHERE GuildId = @iGuildId
                    
                    --길드카페를 폐쇄한다.
                     exec [newclubdb].nmclub3.dbo.USP_N7GuildClubClose @strCafeUrl, 'B0705', NULL, NULL, NULL, NULL, 'B0505'
                     
                     DECLARE @iDelPoint int
                     SELECT  @iDelPoint = Point FROM dbo.guildinfo Where GuildId = @iGuildId

                     INSERT INTO dbo.DelGuildInfo ( GuildName, Point, RegDate ) 
                               VALUES ( @strName, @iDelPoint, GETDATE())

            DELETE FROM dbo.GuildUserState WHERE GuildId = @iGuildId
                     DELETE FROM dbo.Guildinfo WHERE GuildId = @iGuildId AND Name = @strName
                    DELETE FROM dbo.GuildUser WHERE GuildId = @iGuildId
                    DELETE FROM dbo.Notice  WHERE GuildId = @iGuildId
                END
              END
            END

            FETCH NEXT FROM Del_Guild_Cursor INTO @iGuildId, @strName--godsend  수정함 
        END -- while

        CLOSE Del_Guild_Cursor
        DEALLOCATE Del_Guild_Cursor

--원래 이름으로 돌려놓는다. 
Set @strReNameSp = 'sp_rename ''temp_ranking'', ''ranking_guildweek_' + ltrim(rtrim(str(day(dateadd(day, -4, getdate()))))) + ''''
Exec (@strReNameSp)


