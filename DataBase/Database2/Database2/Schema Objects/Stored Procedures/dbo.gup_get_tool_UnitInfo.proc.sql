create PROCEDURE [dbo].[gup_get_tool_UnitInfo]
	@iNickName		nvarchar(16)
AS 
/* 
sutamtwo 
작성일 : 2007년 10월 29일
사용 : UnitStatistics 정보 갱신
*/
SET NOCOUNT ON;
SET XACT_ABORT ON;

DECLARE	@iUnitUID	bigint
,		@iUserUID	bigint
,		@strUserID	nvarchar(20)


SELECT	@iUnitUID = Unituid 
	FROM dbo.gunitnickname WITH (NOLOCK)
		WHERE Nickname = @iNickName

SELECT	@iUserUID = UserUID
	FROM dbo.GUnit	WITH (NOLOCK)
		WHERE UnitUID = @iUnitUID

SELECT	@strUserID = UserID
	FROM account.dbo.Muser WITH (NOLOCK)
		WHERE UserUID = @iUserUID


SELECT @strUserID, a.unitUID, @iNickName, a.unitclass, [exp], [level], a.gamepoint, a.spoint, b.Quantity, c.Spirit, a.vspoint, a.vspointmax
	FROM dbo.Gunit as a WITH (NOLOCK), dbo.GResurrectionStone as b WITH (NOLOCK), dbo.Gspirit as c WITH (NOLOCK)
		WHERE a.UnitUID = @iUnitUID AND a.UnitUID = b.UnitUID AND a.UnitUID = c.UnitUID


