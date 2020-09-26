CREATE FUNCTION [dbo].[zaNick_Login](@strInput [nvarchar](20))
RETURNS [nvarchar](24) WITH SCHEMABINDING
AS 
begin
    declare @strOutput nvarchar(24)

    select @strOutput = Nick
        from dbo.NickNames with (nolock)
            where Login = @strInput

    return @strOutput
end


