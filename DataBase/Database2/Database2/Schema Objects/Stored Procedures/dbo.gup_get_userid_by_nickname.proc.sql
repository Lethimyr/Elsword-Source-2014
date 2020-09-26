CREATE PROCEDURE [dbo].[gup_get_userid_by_nickname]
	@strNickName	nvarchar(16)
,	@iOK			int = 0
AS
SET NOCOUNT ON;

-- 존재하지 않는 닉네임
IF NOT EXISTS (SELECT * FROM dbo.GUnitNickName WITH (NOLOCK) WHERE NickName =  @strNickName)
	BEGIN
		SELECT @iOK = -1
		GOTO END_PROC
	END


DECLARE	@iUnitUID bigint
,		@iUserUID bigint
,		@strUserID nvarchar(20)

SELECT	@iUnitUID = UnitUID 
	FROM dbo.GUnitNickName WITH (NOLOCK)
		WHERE NickName =  @strNickName

SELECT	@strUserID = UserID
	FROM dbo.GUser WITH (NOLOCK)
		WHERE UserUID = (SELECT UserUID FROM dbo.GUnit WITH (NOLOCK) WHERE UnitUID = @iUnitUID)


END_PROC:
SELECT @iOK, RTRIM(@strUserID)


