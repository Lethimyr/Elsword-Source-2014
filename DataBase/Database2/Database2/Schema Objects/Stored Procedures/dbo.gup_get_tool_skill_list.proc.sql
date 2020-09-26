CREATE PROCEDURE [dbo].[gup_get_tool_skill_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;


select a.Name, b.RegDate 
	from dbo.gskillcode as a, dbo.gskill as b with (nolock)
		where b.unituid = @iUnitUID_
			and a.code = b.skillid
	option (maxdop 1)


