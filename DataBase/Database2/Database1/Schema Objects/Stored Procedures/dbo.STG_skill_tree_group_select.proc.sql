CREATE PROCEDURE [dbo].[STG_skill_tree_group_select]
	@iLoginUID_ [int]
AS
set nocount on

select
    CharType
,   GroupID
from dbo.STGSkillTreeGroup
with (nolock)
where
    LoginUID = @iLoginUID_


