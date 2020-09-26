CREATE PROCEDURE [dbo].[gup_get_tool_itemtrade_sell]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT (SELECT nickname FROM game01.dbo.gunitnickname WITH (NOLOCK) WHERE unituid = a.tounituid),  b.itemname, a.quantity, a.regdate 
	FROM [STATISTICS].dbo.ltrade as a WITH (NOLOCK), dbo.gitemcode as b WITH (NOLOCK)
		WHERE fromunituid = @iUnitUID_ AND a.itemid = b.itemcode

--SELECT a.fromunituid, (SELECT nickname FROM game01.dbo.gunitnickname WITH (NOLOCK) WHERE unituid = a.tounituid),  a.itemid, b.itemname, a.quantity, a.regdate 
--	FROM [STATISTICS].dbo.ltrade as a WITH (NOLOCK), dbo.gitemcode as b WITH (NOLOCK)
--		WHERE fromunituid = @iUnitUID_ AND a.itemid = b.itemcode


