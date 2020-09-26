CREATE PROCEDURE [dbo].[up_select_check_list]
	@iLoginUID_ [int]
AS
SET NOCOUNT ON;

DECLARE @vbList varbinary(48)
SELECT @vbList = NULL
SELECT @vbList = List FROM dbo.CheckList(NOLOCK) WHERE LoginUID = @iLoginUID_
IF @vbList is not NULL
    BEGIN
        SELECT @vbList
    END


