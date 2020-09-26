CREATE PROCEDURE [dbo].[gup_get_unit_uid]
	@strNickName_  [nvarchar](16),
    @iUnitUID      [int] = 0
AS
SET NOCOUNT ON;

SELECT @iUnitUID = UnitUID FROM dbo.GUnitNickName(NOLOCK) where NickName = @strNickName_

SELECT @iUnitUID


