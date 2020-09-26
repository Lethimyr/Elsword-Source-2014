CREATE PROCEDURE [dbo].[beup_billing_error_count_select]
	@sdtRegDate_ [smalldatetime]
AS
/*microcat
작성일 : 2007년 10월 19일
사용 : 빌링 에러 코드 카운트 출력
*/
set nocount on;

select @sdtRegDate_ = convert(nvarchar(10), @sdtRegDate_, 120)

select  ErrorName
,       [Count]
    from dbo.BEStatBillingErrorCount with (nolock)
        where RegDate = @sdtRegDate_


