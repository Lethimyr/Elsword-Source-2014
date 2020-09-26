create PROCEDURE [dbo].[gup_check_insert_post_item]
	@FromUnitUID	bigint	
,	@ToUnitNickName	nvarchar(16)
,	@iOK			int = 0

AS
SET NOCOUNT ON;

DECLARE @sdtNow	smalldatetime
,		@ToUnitUID	bigint
,		@strUserID	nvarchar(25)
,		@iUserUID	bigint

SELECT @ToUnitUID = UnitUID
	FROM dbo.GunitNickName WITH (NOLOCK)
		WHERE NickName = @ToUnitNickName
			OPTION (MAXDOP 1)

SELECT	@iUserUID = UserUID
	FROM dbo.GUnit WITH (NOLOCK)
		WHERE UnitUID = @ToUnitUID

SELECT	@strUserID = UserID 
	FROM dbo.GUser WITH (NOLOCK)
		WHERE UserUID = @iUserUID



-- 해당 닉네임이 존재하지 않으면 종료
IF @ToUnitUID IS NULL
	BEGIN	SELECT	@iOK = -99	GOTO END_PROC	END

-- 체험 아이디라면
IF RIGHT(@strUserID, 1) = '/'
	BEGIN	SELECT	@iOK = -96	GOTO END_PROC	END

-- 일반 우편이 30개 넘을 때
IF (SELECT COUNT(*) FROM dbo.GPost WITH (NOLOCK) WHERE ToUnitUID = @ToUnitUID AND SCType = 1 AND Deleted = 0) > 29
	BEGIN	SELECT	@iOK = -98	GOTO END_PROC	END

-- 상대방의 블랙 리스트에 내가 등록 되어있다면
IF EXISTS (SELECT * FROM dbo.GPost_BlackList WITH (NOLOCK) WHERE MyUID = @ToUnitUID AND BlockUID = @FromUnitUID)
	BEGIN	SELECT	@iOK = -97	GOTO END_PROC	END


END_PROC:
SELECT @iOK


