CREATE PROCEDURE [dbo].[gup_get_user_info]
	@iUserUID_ [bigint]
,	@iUserID_  nvarchar(20)
,   @iUSSize   [int] = 0
AS
SET NOCOUNT ON;

DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

-- UserUID가 현재 서버군에 처음 접속한 경우
IF NOT EXISTS( SELECT * FROM dbo.GUser(NOLOCK) where UserUID = @iUserUID_ )
BEGIN
    -- USER 정보 추가.
    INSERT INTO dbo.GUser( UserUID, UserID, USSize, RegDate, DelDate )
        SELECT  @iUserUID_,@iUserID_, 50, @sdtNow, @sdtNow
END

SELECT  @iUSSize = USSize
    FROM dbo.GUser WITH(NOLOCK)
        WHERE UserUID = @iUserUID_

SELECT @iUSSize


