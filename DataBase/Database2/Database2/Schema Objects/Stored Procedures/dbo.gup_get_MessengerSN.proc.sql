CREATE PROCEDURE [dbo].[gup_get_MessengerSN] 
    @iUnitUID_		[bigint]
,	@iMessengerSN	[int] = 0
AS
/* 
작성자 : sutamtwo
작성일 : 2007년 9월 7일
사용 :  캐릭터 선택 시 호출이되며, 호출되면 캐릭터의 UnitUID를 받아 
		넥슨 플러그에 사용될 SN값을 Account.MessengerSN 디비에서 불러온다
결과값 : SN이 0이면 실패, 아닐 경우 성공

*/
SET NOCOUNT ON;

	select @iMessengerSN = SN 
			from account.dbo.messengersn with (nolock)
				where UnitUID = @iUnitUID_			

SELECT @iMessengerSN


