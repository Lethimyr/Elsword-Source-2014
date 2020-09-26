CREATE PROCEDURE [dbo].[fhup_invite_info_get]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : FHInviteInfo의 정보를 리턴
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.fhup_invite_available

SELECT  Type
,       Checked
    FROM dbo.FHInviteInfo WITH (NOLOCK)
        WHERE LoginUIDB = @iLoginUID_ AND @iOK = 0


