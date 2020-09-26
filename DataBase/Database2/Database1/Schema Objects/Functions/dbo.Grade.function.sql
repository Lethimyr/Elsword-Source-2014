CREATE FUNCTION [dbo].[Grade](@iWin [int])
RETURNS [int] AS 
BEGIN
    RETURN
        CASE
            WHEN @iWin <    1 THEN  0
            WHEN @iWin <   31 THEN  1
            WHEN @iWin <  131 THEN  2
            WHEN @iWin <  331 THEN  3
            WHEN @iWin <  531 THEN  4
            WHEN @iWin <  831 THEN  5
            WHEN @iWin < 1331 THEN  6
            WHEN @iWin < 2031 THEN  7
            WHEN @iWin < 2831 THEN  8
            WHEN @iWin < 3831 THEN  9
            WHEN @iWin < 5831 THEN 10
            WHEN @iWin < 8831 THEN 11
            ELSE 12
        END
END


