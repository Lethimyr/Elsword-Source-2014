CREATE FUNCTION [dbo].[ufn_ranking_guildweek](@Guildid [int])
RETURNS [int] AS 
begin
	declare @rank int
	select @rank = rank from dbo.ranking_guildweek(nolock) where Guildid = @Guildid
	return @rank
end


