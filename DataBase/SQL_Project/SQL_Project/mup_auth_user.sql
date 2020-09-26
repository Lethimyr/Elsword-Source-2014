USE [Account]
GO
/****** 개체:  StoredProcedure [dbo].[mup_auth_user]    스크립트 날짜: 03/11/2009 17:48:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[mup_auth_user]
    @bDebugAuth [bit],
	@strUserID_ [nvarchar](20),
	@strPassword_ [nvarchar](20),
	@iAge_ [int],
	@iPublisherSN_ [int],
	@iOK [int] = 0,
	@iUserUID [bigint] = 0,
    @bPlayGuide [bit] = 1,
	@iAuthLevel [int] = 0,
    @bInternalUser [bit] = 0
AS
SET NOCOUNT ON;

DECLARE @strPassword nvarchar(20)
,       @strUserName nvarchar(16)
,		@iAge int
,       @bDeleted bit
,		@sdtEnddate smalldatetime
,		@strReason	nvarchar(100)
,		@bIsRecommend bit
,		@iPublisherSN	int
,		@iSkillOption bit


SELECT  @sdtEnddate = getdate()
,		@strReason = '제재 이유'

SELECT  @iUserUID = UserUID
,       @strPassword = Password
,       @strUserName = UserName
,		@iAge = Age
,       @bPlayGuide = PlayGuide
,       @bDeleted = Deleted
,		@bIsRecommend = IsRecommend
,		@iPublisherSN = PublisherSN
    FROM dbo.MUser WITH(NOLOCK)
        WHERE UserID = @strUserID_


-- 스킬 옵션 얻어가기
SELECT @iSkillOption = SkillOption 
	FROM dbo.MUserSkillOption WITH (NOLOCK)
		WHERE USERUID = @iUserUID



IF @iUserUID = 0 --유저가 존재하지 않음
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF @bDeleted = 1 --삭제된 유저임
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @bDebugAuth = 1
BEGIN
	IF @strPassword_ <> @strPassword --틀린 팻흐워두
		BEGIN    SELECT @iOK = -3    GOTO END_PROC    END
END

-- 나이는 변할 수 있다
IF @iAge_ <> @iAge
BEGIN
	UPDATE dbo.MUser SET Age = @iAge_ WHERE UserID = @strUserID_
END

SELECT @iAuthLevel = AuthLevel FROM dbo.MUserAuthority( NOLOCK ) WHERE UserUID = @iUserUID

-- 일반 유저라면, 계정 블럭 상태인가 파악한다
IF @iAuthLevel = 0	
	BEGIN
		SELECT @iAuthLevel = authlevel, @sdtEndDate = enddate, @strReason = reason
			from dbo.userauthlevel with (nolock)
				where useruid = @iUserUID
	END

-- 블럭상태가 ON인 유저라면 EndDate가 오늘보다 이전 날짜인지 판단
IF @iAuthLevel < 0
	BEGIN
		IF @sdtEnddate < getdate()
			BEGIN
				UPDATE dbo.userauthlevel 
					SET authlevel = 0
						WHERE userUID = @iuserUID

				SELECT @iAuthLevel = 0
			END

	END


-- 사내 또는 넥슨 유저인지 식별한다.
IF EXISTS( SELECT * FROM dbo.MInternalID WITH( NOLOCK ) WHERE ID = @strUserID_ )
BEGIN
    SET @bInternalUser = 1
END

END_PROC:
--마지막 행 추가 0
SELECT @iOK, @iUserUID, @strUserName, @iAuthLevel, @bPlayGuide, @bInternalUser, @sdtEnddate, @strReason, @bIsRecommend, ISNULL(@iSkillOption, 0)