CREATE PROCEDURE [dbo].[fhup_invite_user_get]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : FHInviteUser의 정보를 리턴
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.fhup_invite_available

declare @iRCount int
select  @iRCount = 0
SELECT  @iRCount = RCount
    FROM dbo.FHInviteUser WITH (NOLOCK)
        WHERE LoginUIDB = @iLoginUID_ AND @iOK = 0

select @iRcount


