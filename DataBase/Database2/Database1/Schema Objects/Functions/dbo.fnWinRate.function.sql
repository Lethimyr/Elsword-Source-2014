CREATE FUNCTION [dbo].[fnWinRate](@iWin_ [int], @iLose_ [int])
RETURNS [real] WITH SCHEMABINDING
AS 
BEGIN
    IF( @iWin_ + @iLose_ = 0 ) RETURN 0

    RETURN ( CAST(@iWin_ AS REAL) / (@iWin_ + @iLose_) ) * 100
END


