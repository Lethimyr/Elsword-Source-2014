CREATE PROCEDURE [dbo].[up_get_stat_mode_count]
	@sdtNow_ [smalldatetime]
AS
/*microcat
작성일 : 2006년 12월 11일
사용 : 통계툴
*/
SET NOCOUNT ON;

SELECT a.ModeName, b.RegDate, b.ModeCount
    FROM dbo.DescModeCount as a(NOLOCK)
        JOIN dbo.StatModeCount as b(NOLOCK) ON a.ModeCode = b.ModeCode AND b.RegDate = @sdtNow_


