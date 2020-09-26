CREATE PROCEDURE [dbo].[gup_get_unit_info_by_useruid]
	@iUserUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  a.UnitUID
,       a.UnitClass
,       a.[Exp]
,       a.[Level]
,       a.GamePoint
,       a.VSPoint
,       a.VSPointMax
,       a.BaseHP
,       a.AtkPhysic
,       a.AtkMagic
,       a.DefPhysic
,       a.DefMagic
,       a.SPoint
,       b.NickName
,       a.Win
,       a.Lose
,       a.LastPosition
    FROM dbo.GUnit as a WITH(NOLOCK)
        JOIN dbo.GUnitNickName as b WITH(NOLOCK)
            ON a.UserUID = @iUserUID_ AND a.Deleted = 0 AND a.UnitUID = b.UnitUID --삭제된 유저 정보 읽어갈까봐...


