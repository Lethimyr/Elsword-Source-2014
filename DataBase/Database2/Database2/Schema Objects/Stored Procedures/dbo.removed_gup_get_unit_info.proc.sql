CREATE PROCEDURE [dbo].[removed_gup_get_unit_info]
	@iUserUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  a.UnitUID
,       a.UnitClass
,       a.[Exp]
,       a.GamePoint
,       a.ISize
,       a.QSSize
,       a.[STR]
,       a.DEX
,       a.[INT]
,       a.VIT
,       a.LUK
,       a.SPoint
,       b.NickName
    FROM dbo.GUnit as a WITH(NOLOCK)
        JOIN dbo.GUnitNickName as b WITH(NOLOCK)
            ON a.UserUID = @iUserUID_ AND a.Deleted = 0 AND a.UnitUID = b.UnitUID --삭제된 유저 정보 읽어갈까봐...


