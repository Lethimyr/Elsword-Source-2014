CREATE PROCEDURE [dbo].[STG_skill_tree_id_select]
	@iLoginUID_ [int]
AS
set nocount on

select
    CharType
,   Promotion
,   SkillID
from dbo.STGSkillTreeID
with (nolock)
where
    LoginUID = @iLoginUID_


