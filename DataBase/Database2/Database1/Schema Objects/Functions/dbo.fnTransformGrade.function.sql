CREATE FUNCTION [dbo].[fnTransformGrade](@iExp0 [int], @iExp1 [bigint], @iExp2 [bigint], @iExp3 [bigint])
RETURNS [int] WITH SCHEMABINDING
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

    IF( @iGradePoint < 4060      )    BEGIN    RETURN 0     END
    IF( @iGradePoint < 11700     )    BEGIN    RETURN 1     END
    IF( @iGradePoint < 45600     )    BEGIN    RETURN 2     END
    IF( @iGradePoint < 152180    )    BEGIN    RETURN 3     END
    IF( @iGradePoint < 391600    )    BEGIN    RETURN 4     END
    IF( @iGradePoint < 2550600   )    BEGIN    RETURN 5     END
    IF( @iGradePoint < 10054200  )    BEGIN    RETURN 6     END
    IF( @iGradePoint < 29850200  )    BEGIN    RETURN 7     END
    IF( @iGradePoint < 68394200  )    BEGIN    RETURN 8     END
    IF( @iGradePoint < 114354200 )    BEGIN    RETURN 9     END
    IF( @iGradePoint < 177810200 )    BEGIN    RETURN 10    END

    RETURN 11
END


