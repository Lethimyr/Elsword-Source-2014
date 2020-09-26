CREATE PROCEDURE [dbo].[fhup_invite_available]
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : 해당 이벤트를 계속 진행해도 되는 것인지...
*/
SET NOCOUNT ON;

DECLARE @iOK int
SELECT @iOK = CASE WHEN GetDate() >= '070601' THEN -1 ELSE 0 END

RETURN @iOK


