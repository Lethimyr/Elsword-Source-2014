CREATE PROCEDURE [dbo].[fhup_invite_item_supply]
	@iLoginUID_ [int],
	@iType_ [tinyint],
	@iOK [int] = 0
AS
set nocount on;

EXEC @iOK = dbo.fhup_invite_available
IF @iOK = -1
    BEGIN    GOTO END_PROC    END

DECLARE @strLogin nvarchar(20)
SELECT  @strLogin = Login
    FROM dbo.Users WITH (NOLOCK)
        WHERE LoginUID = @iLoginUID_
IF @strLogin IS NULL
    BEGIN    SELECT @iOK = -1    goto end_proc    END

DECLARE @strSQL nvarchar(4000)
SELECT  @strSQL =   CASE @iType_
                        WHEN 1 THEN N'38880,7,'
                        WHEN 2 THEN N'38850,1,38970,1,38880,7,'
                        WHEN 3 THEN N'42370,1,'
                        WHEN 4 THEN N'42350,-1,42370,1,'
                    END

end_proc:
if @iOK = 0
    begin
        EXEC dbo.up_insert_item @strLogin, @strSQL
    end
else
    begin
        select @iOK
    end


