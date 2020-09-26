CREATE PROCEDURE [dbo].[up_web_is_las_character]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_web_is_las_character'

SET NOCOUNT ON

IF EXISTS(SELECT * FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = 3)
    BEGIN
        SELECT 0
        RETURN
    END

SELECT -1
RETURN


