CREATE FUNCTION [dbo].[zdGrade_Win](@iInput [int])
RETURNS [tinyint] WITH SCHEMABINDING
AS 
begin
    declare @iOutput tinyint

    select @iOutput =
        case
            when @iInput < 1    then 0
            when @iInput < 31   then 1
            when @iInput < 131  then 2
            when @iInput < 331  then 3
            when @iInput < 531  then 4
            when @iInput < 831  then 5
            when @iInput < 1331 then 6
            when @iInput < 2031 then 7
            when @iInput < 2831 then 8
            when @iInput < 3831 then 9
            when @iInput < 5831 then 10
            when @iInput < 8831 then 11
            else 12
        end

    return @iOutput
END


