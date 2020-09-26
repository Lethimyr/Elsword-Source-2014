CREATE FUNCTION [dbo].[IPs_IPn](@iIP [bigint])
RETURNS [nvarchar](15) WITH SCHEMABINDING
AS 
begin

declare
    @iIP1 tinyint
,   @iIP2 tinyint
,   @iIP3 tinyint
,   @iIP4 tinyint

select
    @iIP1 = ( @iIP ) / 16777216
,   @iIP2 = ( @iIP - 16777216 * convert( bigint, @iIP1 ) ) / 65536
,   @iIP3 = ( @iIP - 16777216 * convert( bigint, @iIP1 ) - 65536 * @iIP2 ) / 256
,   @iIP4 = ( @iIP - 16777216 * convert( bigint, @iIP1 ) - 65536 * @iIP2 - 256 * @iIP3 ) / 1

return
    convert(nvarchar(3), @iIP1)
+   N'.'
+   convert(nvarchar(3), @iIP2)
+   N'.'
+   convert(nvarchar(3), @iIP3)
+   N'.'
+   convert(nvarchar(3), @iIP4)

end


