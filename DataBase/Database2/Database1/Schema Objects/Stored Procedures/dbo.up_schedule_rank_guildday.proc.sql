CREATE PROCEDURE [dbo].[up_schedule_rank_guildday]
AS
/*
매일 1회
*/

SET NOCOUNT ON
SET XACT_ABORT ON

declare @strDay nvarchar(2), @strSQL nvarchar(1024)

SET @strDay = DateName(d, getdate() - 1)
SET @strSQL = 'ranking_guildday_' + @strDay
IF EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = @strSQL)
BEGIN
    RETURN -1
END

-- 순위 임시 테이블 생성
CREATE TABLE ranking_guildday_new (
    Rank int IDENTITY (1, 1) Primary Key,
    GuildId int,
    [Name] nvarchar(12),
    Point int,
    Mark int,
    NumMembers int,
    LoadLogin nvarchar(20),
    NickName nvarchar (24),
    Increase int,
    cafe_url nvarchar(20)
) ON [PRIMARY]

INSERT INTO dbo.ranking_guildday_new (GuildId, [Name], Point, Mark, NumMembers, LoadLogin, NickName, Increase, cafe_url)
    SELECT a.GuildId, a.[Name], a.Point, a.Mark, a.NumMembers, a.LoadLogin, b.Nick, 0, a.cafe_url 
        FROM dbo.GuildInfo AS a(nolock) LEFT OUTER JOIN dbo.NickNames AS b(nolock) ON a.LoadLogin = b.Login 
            WHERE a.approval = 1 ORDER BY a.Point DESC

SET @strDay = DateName(d, getdate())
SET @strSQL = 'create unique nonclustered index IX_ranking_guildday_' + @strDay + ' on ranking_guildday_new(GuildID)'
EXEC sp_executesql @strSQL

IF EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = 'ranking_guildday')
BEGIN
    UPDATE a SET Increase = b.Rank - a.Rank FROM dbo.ranking_guildday_new as a join dbo.ranking_guildday as b on a.GuildId = b.GuildId
    SET @strDay = DateName(d, getdate() - 1)
    SET @strSQL = 'ranking_guildday_' + @strDay
    EXEC sp_rename 'ranking_guildday', @strSQL
END

EXEC sp_rename 'ranking_guildday_new', 'ranking_guildday'

SET @strDay = DateName(d, getdate() - 9)
SET @strSQL = 'ranking_guildday_' + @strDay
IF EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = @strSQL)
BEGIN
    SET @strSQL = 'DROP TABLE ranking_guildday_' + @strDay
    EXEC sp_executesql @strSQL
END


