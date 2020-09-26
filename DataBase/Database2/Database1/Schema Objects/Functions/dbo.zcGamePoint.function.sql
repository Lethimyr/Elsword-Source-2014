CREATE FUNCTION [dbo].[zcGamePoint](@iInput [int])
RETURNS [int] WITH SCHEMABINDING
AS
begin
    declare
        @iMax int
    select
        @iMax = 999999

    select
        @iMax = Variable1
    from dbo.GlobalVariables
    with (nolock)
    where
        No = 1

    return
        case
            when @iInput > @iMax then @iMax
            when @iInput < 0 then 0
            else @iInput
        end
end


