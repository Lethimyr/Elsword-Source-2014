CREATE PROCEDURE [dbo].[up_schedule_rank_guildweek]
AS
/*
매일 1회
*/

SET NOCOUNT ON
SET XACT_ABORT ON

declare @strDay nvarchar(2), @strSQL nvarchar(1024)

SET @strDay = DateName(d, getdate() - 1)
SET @strSQL = 'ranking_guildweek_' + @strDay
IF EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = @strSQL)
BEGIN
    RETURN -1
END

SET @strDay = DateName(d, getdate() - 6)
SET @strSQL = 'ranking_guildday_' + @strDay
IF NOT(EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = @strSQL) AND EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = 'ranking_guildday'))
BEGIN
    RETURN -2
END

-- 순위 임시 테이블 생성
CREATE TABLE ranking_guildweek_new (
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

SET @strDay = DateName(d, getdate() - 6)
SET @strSQL = 'INSERT INTO dbo.ranking_guildweek_new(GuildId, [Name], Point, Mark, NumMembers, LoadLogin, NickName, Increase, cafe_url) '
SET @strSQL = @strSQL + 'SELECT a.GuildId, a.[Name], a.Point - b.Point, a.Mark, a.NumMembers, a.LoadLogin, c.Nick, 0, a.cafe_url '
SET @strSQL = @strSQL + 'FROM dbo.ranking_guildday AS a(NOLOCK) LEFT OUTER JOIN dbo.NickNames AS c(NOLOCK) ON a.LoadLogin = c.Login '
SET @strSQL = @strSQL + 'JOIN dbo.ranking_guildday_' + @strDay + ' AS b(NOLOCK) ON a.GuildId = b.GuildID '
SET @strSQL = @strSQL + 'ORDER BY a.Point - b.Point DESC'
EXEC sp_executesql @strSQL

SET @strDay = DateName(d, getdate())
SET @strSQL = 'create unique nonclustered index IX_ranking_guildweek_' + @strDay + ' on ranking_guildweek_new(GuildID)'
EXEC sp_executesql @strSQL

IF EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = 'ranking_guildweek')
BEGIN
    UPDATE a SET Increase = b.Rank - a.Rank FROM dbo.ranking_guildweek_new as a join dbo.ranking_guildweek as b on a.GuildId = b.GuildId
    SET @strDay = DateName(d, getdate() - 1)
    SET @strSQL = 'ranking_guildweek_' + @strDay
    EXEC sp_rename 'ranking_guildweek', @strSQL
END

EXEC sp_rename 'ranking_guildweek_new', 'ranking_guildweek'

SET @strDay = DateName(d, getdate() - 6)
SET @strSQL = 'ranking_guildweek_' + @strDay
IF EXISTS(SELECT * FROM dbo.SysObjects(NOLOCK) WHERE [Name] = @strSQL)
BEGIN
    SET @strSQL = 'DROP TABLE ranking_guildweek_' + @strDay
    EXEC sp_executesql @strSQL
END


