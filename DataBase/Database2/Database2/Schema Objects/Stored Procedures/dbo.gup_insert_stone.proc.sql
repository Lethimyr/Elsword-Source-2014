/* 현재 사용되고있지 않음 */
CREATE PROCEDURE [dbo].[gup_insert_stone]
	@strNick	nvarchar(16)
,	@iQuantity	smallint
AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

DECLARE @iUnitUID	bigint
SELECT @iUnitUID = unitUID
	FROM dbo.GUnitNickname
		WHERE nickname = @strNick

	IF NOT EXISTS (SELECT * FROM dbo.GResurrectionstone WHERE unitUID = @iUnitUID)
	BEGIN
		INSERT INTO dbo.GResurrectionstone (UnitUID, Quantity)
			SELECT @iUnitUID, @iQuantity
	END

		UPDATE dbo.GResurrectionstone
			SET Quantity = @iQuantity
				WHERE UnitUID = @iUnitUID

SELECT * FROM DBO.GResurrectionstone
	WHERE UNITUID = @iUnitUID


