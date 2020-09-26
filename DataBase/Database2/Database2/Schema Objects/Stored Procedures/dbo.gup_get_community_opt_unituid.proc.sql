CREATE PROCEDURE [dbo].[gup_get_community_opt_unituid]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

DECLARE @iValue1 tinyint
DECLARE @iValue2 tinyint
DECLARE @iValue3 tinyint
DECLARE @iValue4 tinyint

SET @iValue1 = (SELECT  CodeNo FROM dbo.GDenyOption WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestionNo = 1)
IF @iValue1 IS NULL
BEGIN
	SET @iValue1 = 1
END

SET @iValue2 = (SELECT  CodeNo FROM dbo.GDenyOption WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestionNo = 2)
IF @iValue2 IS NULL
BEGIN
	SET @iValue2 = 1
END

SET @iValue3 = (SELECT  CodeNo FROM dbo.GDenyOption WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestionNo = 3)
IF @iValue3 IS NULL
BEGIN
	SET @iValue3 = 1
END

SET @iValue4 = (SELECT  CodeNo FROM dbo.GDenyOption WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestionNo = 4)
IF @iValue4 IS NULL
BEGIN
	SET @iValue4 = 1
END

SELECT @iValue1, @iValue2, @iValue3, @iValue4


