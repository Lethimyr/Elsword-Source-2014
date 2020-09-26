CREATE PROCEDURE [dbo].[gup_get_blacklist_by_unituid]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  a.UnitUID2
,       b.NickName
    FROM dbo.GFriendList as a WITH(NOLOCK)
        JOIN dbo.GUnitNickName as b WITH(NOLOCK)
            ON a.UnitUID1 = @iUnitUID_ AND a.UnitUID2 = b.UnitUID AND type = 4

--해당 UnitUID의 블랙리스트유저UnitUID와 닉네임까지 얻기


