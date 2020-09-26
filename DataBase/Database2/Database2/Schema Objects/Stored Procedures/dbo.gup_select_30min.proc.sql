CREATE PROCEDURE [dbo].[gup_select_30min]
	@iUnitUID	BIGINT
,	@iOK		[int] = 0
AS 
/* 
sutamtwo
작성일 : 2008년 05월 19일
사용 : 30분 접속 이벤트 (SELECT 용)
*/
SET NOCOUNT ON;
SET XACT_ABORT ON;

	SELECT EVENTTYPE, REGDATE
		FROM dbo.GIs30min WITH (NOLOCK)
			WHERE UnitUID = @iUnitUID


