CREATE FUNCTION [dbo].[ufn_ranking_guildday](@Guildid [int])
RETURNS [int] AS 
begin
	declare @rank int
	select @rank = rank from dbo.ranking_guildday(NOLOCK) where Guildid = @Guildid
	return @rank
end


