CREATE PROCEDURE [dbo].[gup_pointbag_luckyday_get]
	@sdtRegDate_ [smalldatetime]
AS
/*microcat
작성일 : 2007년 6월 14일
사용 : 포인트몰의 특정일을 리턴
*/
SET NOCOUNT ON;

select @sdtRegDate_ = convert(nvarchar(8), @sdtRegDate_, 120) + N'01'

select RegDate
    from dbo.GPointBagLuckyDayItem with (nolock)
        where RegDate between @sdtRegDate_ and dateadd(mm, 1, @sdtRegDate_) - 1
            Group by RegDate


