CREATE PROCEDURE [dbo].[gup_get_post_item_new]
	@UnitUID	bigint	
,	@iOK		int = 0
AS
SET NOCOUNT ON;

	SELECT a.PostNo, ISNULL(b.nickname,'__DELETED__'), a.toUnitUID, a.Quantity, a.ScType, a.ScIndex, a.RegDate, a.IsReceive, a.IsRead, a.title, a.Message, c.ELevel, D.socket1, D.socket2, D.socket3, D.socket4, E.Attribute1, E.Attribute2
		FROM dbo.GPost AS A WITH (NOLOCK)
			JOIN dbo.GUnitNickName AS B WITH (NOLOCK)
				ON a.ToUnitUID = @UnitUID AND a.DELETED = 0 
					AND a.FromUnitUID = b.UnitUID
			LEFT OUTER JOIN dbo.GPost_Enchant AS C WITH (NOLOCK)
				ON A.PostNo = C.PostNo
			LEFT OUTER JOIN dbo.GPost_Socket AS D WITH (NOLOCK)
				ON A.PostNo = D.PostNo
			LEFT OUTER JOIN dbo.GPost_Attribute AS E WITH (NOLOCK)
				ON A.PostNo = E.PostNo


