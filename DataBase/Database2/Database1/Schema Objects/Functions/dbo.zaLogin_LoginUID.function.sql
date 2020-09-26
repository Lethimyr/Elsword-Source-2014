CREATE FUNCTION [dbo].[zaLogin_LoginUID](@iInput [int])
RETURNS [nvarchar](20) WITH SCHEMABINDING
AS 
begin
    declare @strOutput nvarchar(20)

    select @strOutput = Login
        from dbo.Users with (nolock)
            where LoginUID = @iInput

    return @strOutput
end


