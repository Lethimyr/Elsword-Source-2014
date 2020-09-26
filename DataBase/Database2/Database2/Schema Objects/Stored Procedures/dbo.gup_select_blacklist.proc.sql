/*	sutamtwo
	2008-01-31
	블랙 리스트에 얻어오기
*/

CREATE PROCEDURE [dbo].[gup_select_blacklist]
(
	@iMyUID		bigint	
,	@iOK		int = 0
)

AS

SET NOCOUNT ON ;

SELECT a.BlockUID, b.Nickname
	FROM dbo.GBlackList	as a WITH (NOLOCK) , dbo.GUnitNickname AS b WITH (NOLOCK)
		WHERE a.MyUID = @iMyUID and a.blockuid = b.unituid


