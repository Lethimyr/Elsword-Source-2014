USE [Account]
GO
/****** 개체:  StoredProcedure [dbo].[gup_get_tool_login_history]    스크립트 날짜: 03/11/2009 17:37:48 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROCEDURE [dbo].[gup_get_tool_login_history]
	@strSession_	[nvarchar](20)
,	@strIP_			[nvarchar](15)
,	@iOK			[int] = 0

AS
SET NOCOUNT ON;

DECLARE @sdtNow smalldatetime
SELECT @sdtNow = getdate()


BEGIN TRAN
	
	insert into account.dbo.TuserLoginHistory (Session, IP, RegDate)
		select @strSession_, @strIP_, @sdtNow

    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -1    GOTO FAIL_TRAN    END

COMMIT TRAN


GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK