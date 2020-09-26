CREATE PROCEDURE [dbo].[fhup_invite_temp_check]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : 추천할 수 있는 대상자인지 확인
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.fhup_invite_available
IF @iOK = -1
    BEGIN    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.FHInviteTemp WITH (NOLOCK) WHERE LoginUIDA = @iLoginUID_)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

if exists(select * from dbo.FHInviteTemp with (nolock) where LoginUIDA = @iLoginUID_ and Invited = 1)
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

END_PROC:
SELECT @iOK


