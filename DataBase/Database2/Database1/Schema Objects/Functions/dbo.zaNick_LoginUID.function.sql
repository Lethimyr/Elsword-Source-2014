CREATE FUNCTION [dbo].[zaNick_LoginUID](@iInput [int])
RETURNS [nvarchar](24) WITH SCHEMABINDING
AS 
begin
    declare @strOutput nvarchar(24)
,           @strLogin nvarchar(20)

    select  @strLogin = Login
        from dbo.Users with (nolock)
            where LoginUID = @iInput

    select  @strOutput = Nick
        from dbo.Nicknames with (nolock)
            where Login = @strLogin

    return @strOutput
end


