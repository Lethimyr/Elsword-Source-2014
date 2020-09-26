CREATE PROCEDURE [dbo].[gup_get_tool_itemtrade_buy]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT (SELECT nickname FROM game01.dbo.gunitnickname WITH (NOLOCK) WHERE unituid = a.fromunituid),  b.itemname, a.quantity, a.regdate 
	FROM [STATISTICS].dbo.ltrade as a WITH (NOLOCK), dbo.gitemcode as b WITH (NOLOCK)
		WHERE tounituid = @iUnitUID_ AND a.itemid = b.itemcode


