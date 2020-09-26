CREATE FUNCTION [dbo].[fnCalcLotto](@iLotto1 [int], @iLotto2 [int], @iLotto3 [int], @iLotto4 [int], @iLotto5 [int])
RETURNS [int] WITH SCHEMABINDING
AS 
BEGIN
    RETURN POWER(2, @iLotto1 - 1) + POWER(2, @iLotto2 - 1) + POWER(2, @iLotto3 - 1) + POWER(2, @iLotto4 - 1) + POWER(2, @iLotto5 - 1)
END


