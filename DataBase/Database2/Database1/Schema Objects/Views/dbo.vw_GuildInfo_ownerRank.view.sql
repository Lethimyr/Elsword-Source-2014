CREATE VIEW [dbo].[vw_GuildInfo_ownerRank]
AS
SELECT     a.GuildId, a.Name, a.LoadLogin, a.Mark, a.Point, a.NumMembers, ISNULL(dbo.ufn_ranking_guildday(a.GuildId), 0) AS rankday, 
                      ISNULL(dbo.ufn_ranking_guildweek(a.GuildId), 0) AS rankweek, b.Login, a.Approval, a.cafe_url, a.Explanation
FROM         dbo.GuildInfo AS a WITH (NOLOCK) INNER JOIN
                      dbo.GuildUser AS b WITH (NOLOCK) ON a.GuildId = b.GuildId


