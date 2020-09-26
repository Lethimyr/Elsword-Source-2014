CREATE FUNCTION [dbo].[zaLoginUID_Login](@strInput [nvarchar](20))
RETURNS [int] WITH SCHEMABINDING
AS 
begin
    declare @iOutput int

    select @iOutput = LoginUID
        from dbo.Users with (nolock)
            where Login = @strInput

    return @iOutput
end


