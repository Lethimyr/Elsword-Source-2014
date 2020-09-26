CREATE FUNCTION [dbo].[zaLogin_Nick](@strInput [nvarchar](24))
RETURNS [nvarchar](20) WITH SCHEMABINDING
AS 
begin
    declare @strOutput nvarchar(20)

    select @strOutput = Login
        from dbo.NickNames with (nolock)
            where Nick = @strInput

    return @strOutput
end


