CREATE PROCEDURE [dbo].[gup_get_post_item]
	@UnitUID	bigint	
,	@iOK		int = 0
AS
SET NOCOUNT ON;

	SELECT a.PostNo, ISNULL(b.nickname,'__DELETED__'), a.FromUnitUID, a.Quantity, a.ScType, a.ScIndex, a.RegDate,	a.Message
		FROM dbo.GPost AS A WITH (NOLOCK)
			JOIN dbo.GUnitNickName AS B WITH (NOLOCK)
				ON a.ToUnitUID = @UnitUID AND a.DELETED = 0 
					AND a.FromUnitUID = b.UnitUID


