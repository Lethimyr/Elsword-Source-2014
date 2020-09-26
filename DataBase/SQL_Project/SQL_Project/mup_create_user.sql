USE [Account]
GO
/****** 개체:  StoredProcedure [dbo].[mup_create_user]    스크립트 날짜: 03/12/2009 15:05:26 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[mup_create_user]
	@strUserID_ [nvarchar](20),
	@strPassword_ [nvarchar](20),
	@strUserName_ [nvarchar](16),
	@iGuestUser_ [bit],
	@iOK [int] = 0,
	@iUserUID [bigint] = 0
AS
SET NOCOUNT ON;

IF EXISTS(SELECT * FROM dbo.Muser WITH(NOLOCK) WHERE UserID = @strUserID_ )
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END --중복되는 유저ID 있삼
 
DECLARE @sdtNow smalldatetime
SELECT @sdtNow = GetDate()

BEGIN TRAN
    INSERT INTO   dbo.MUser(
                            UserID
,                           Password
,                           UserName
,                           RegDate
,                           DelDate
,                           PlayGuide
,							GuestUser
                           )
        SELECT  @strUserID_
,               @strPassword_
,               @strUserName_
,               @sdtNow
,               @sdtNow
,               1
,				@iGuestUser_

    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1 --이건 한개만 입력되어야 하삼
    BEGIN    SELECT @iOK = -11   GOTO FAIL_TRAN    END

    SELECT @iUserUID = @@IDENTITY
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iUserUID