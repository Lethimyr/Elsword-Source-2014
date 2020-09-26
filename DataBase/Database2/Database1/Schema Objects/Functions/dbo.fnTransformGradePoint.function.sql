CREATE FUNCTION [dbo].[fnTransformGradePoint](@iExp0 [bigint], @iExp1 [bigint], @iExp2 [bigint], @iExp3 [bigint])
RETURNS [bigint] WITH SCHEMABINDING
AS 
BEGIN
    DECLARE @iGradePoint bigint
    SET @iGradePoint = 0
    DECLARE @iMax bigint
    SET @iMax = @iExp0

    IF( @iMax < @iExp1 )    SET @iMax = @iExp1
    IF( @iMax < @iExp2 )    SET @iMax = @iExp2
    IF( @iMax < @iExp3 )    SET @iMax = @iExp3

    SET @iGradePoint = ( @iExp0 + @iExp1 + @iExp2 + @iExp3 + ( @iMax * 2 ) ) / 3

    RETURN @iGradePoint 
END


