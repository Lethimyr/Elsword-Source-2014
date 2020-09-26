CREATE FUNCTION [dbo].[zaLoginUID_Nick](@strInput [nvarchar](24))
RETURNS [int] WITH SCHEMABINDING
AS 
begin
    declare @iOutput int
,           @strLogin nvarchar(20)

    select  @strLogin = Login
        from dbo.Nicknames with (nolock)
            where Nick = @strInput

    select  @iOutput = LoginUID
        from dbo.Users with (nolock)
            where Login = @strLogin

    return @iOutput
end


