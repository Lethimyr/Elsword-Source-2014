create PROCEDURE [dbo].[gup_get_post_blacklist]
	@iUnitUID	[bigint]
AS
SET NOCOUNT ON;

SELECT B.NickName
	FROM dbo.GPost_BlackList AS A WITH(NOLOCK)
		JOIN dbo.GUnitNickName AS B WITH(NOLOCK)
			ON a.MyUID = @iUnitUID
				AND a.BlockUID = b.UnitUID


