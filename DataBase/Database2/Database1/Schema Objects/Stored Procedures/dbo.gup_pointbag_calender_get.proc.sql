CREATE PROCEDURE [dbo].[gup_pointbag_calender_get]
	@iLoginUID_ [int],
	@sdtRegDate_ [smalldatetime]
AS
/*microcat
작성일 : 2007년 4월 12일
사용 : 유저의 포인트백 달력을 얻는다.
*/
SET NOCOUNT ON;

DECLARE @sdtFirst smalldatetime
SELECT  @sdtFirst = CONVERT(nvarchar(8), @sdtRegDate_, 120) + N'01'

SELECT  RegDate
,       case when Datepart(week, RegDate) = 53 and DatePart(week, @sdtFirst) = 1 then DatePart(week, @sdtFirst) else Datepart(week, RegDate) - DatePart(week, @sdtFirst) + 1 end
,       SealType
    FROM dbo.GPointBagCalender WITH (NOLOCK)
        WHERE LoginUID = @iLoginUID_ AND RegDate BETWEEN @sdtFirst - DatePart(dw, @sdtFirst) + 1 AND DateAdd(mm, 1, @sdtFirst) - 1


