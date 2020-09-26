/*	sutamtwo
	2008-10-07
	보유중인 (클리어 한)
	PERIOD 값이 0이면 영구, 나머지 숫자면 더해서 종료일 계산
*/

create PROCEDURE [dbo].[gup_get_title_list]
(
	@iUnitUID	bigint
)

AS

SET NOCOUNT ON ;


	SELECT TitleID, EndDate, IsHang
		FROM dbo.GTitle_Complete WITH (NOLOCK)
			WHERE UnitUID = @iUnitUID


