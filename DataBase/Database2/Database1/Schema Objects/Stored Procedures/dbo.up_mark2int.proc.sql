CREATE PROCEDURE [dbo].[up_mark2int]
	@iMark_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

DECLARE @iColor    int,
        @iShild    int,
        @iBack     int

SET @iColor = @iMark_ / 10000
SET @iShild = ( @iMark_ - ( @iColor * 10000 ) ) / 100
SET @iBack = (@iMark_ - ( @iColor * 10000 ) - ( @iShild * 100 ) )

RETURN ( @iColor + @iShild * 256 + @iBack * 65536 )


