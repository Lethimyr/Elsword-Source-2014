CREATE PROCEDURE [dbo].[up_int2mark]
	@iMark_ [int],
	@strMark_ [nvarchar](6) OUTPUT
AS
set nocount on
set transaction isolation level read uncommitted

DECLARE @iColor    int,
        @iShild    int,
        @iBack     int

SET @iBack = @iMark_ / 65536
SET @iShild = ( @iMark_ - ( @iBack * 65536 ) ) / 256
SET @iColor = @iMark_ - @iBack * 65536 - @iShild * 256

DECLARE @strMark nvarchar(6)
SET @strMark = ''

IF @iColor = 0
    SET @strMark = @strMark + '00'
ELSE IF @iColor < 10
    SET @strMark = @strMark + '0' + CONVERT(nvarchar(1), @iColor)
ELSE 
    SET @strMark = @strMark + CONVERT(nvarchar(2), @iColor)

IF @iShild = 0
    SET @strMark = @strMark + '00'
ELSE IF @iShild < 10
    SET @strMark = @strMark + '0' + CONVERT(nvarchar(1), @iShild)
ELSE 
    SET @strMark = @strMark + CONVERT(nvarchar(2), @iShild)

IF @iBack = 0
    SET @strMark = @strMark + '00'
ELSE IF @iBack < 10
    SET @strMark = @strMark + '0' + CONVERT(nvarchar(1), @iBack)
ELSE 
    SET @strMark = @strMark + CONVERT(nvarchar(2), @iBack)

SET @strMark_ = @strMark
RETURN @strMark_


