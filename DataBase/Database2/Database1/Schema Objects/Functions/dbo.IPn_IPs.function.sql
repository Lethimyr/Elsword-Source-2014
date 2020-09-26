CREATE FUNCTION [dbo].[IPn_IPs](@strIP [nvarchar](15))
RETURNS [bigint] WITH SCHEMABINDING
AS 
begin

declare
    @iStart tinyint
,   @iEnd tinyint
,   @iIP1 tinyint
,   @iIP2 tinyint
,   @iIP3 tinyint
,   @iIP4 tinyint
select
    @iStart = 1
,   @iEnd = 1
,   @iEnd = charindex(N'.', @strIP, @iStart + 1)
,   @iIP1 = substring(@strIP, @iStart, @iEnd - @iStart)
,   @iStart = @iEnd + 1
,   @iEnd = charindex(N'.', @strIP, @iStart + 1)
,   @iIP2 = substring(@strIP, @iStart, @iEnd - @iStart)
,   @iStart = @iEnd + 1
,   @iEnd = charindex(N'.', @strIP, @iStart + 1)
,   @iIP3 = substring(@strIP, @iStart, @iEnd - @iStart)
,   @iStart = @iEnd + 1
,   @iEnd = 16
,   @iIP4 = substring(@strIP, @iStart, @iEnd - @iStart)

return
    convert(bigint, @iIP1) * 16777216
+   @iIP2 * 65536
+   @iIP3 * 256
+   @iIP4 * 1

end


