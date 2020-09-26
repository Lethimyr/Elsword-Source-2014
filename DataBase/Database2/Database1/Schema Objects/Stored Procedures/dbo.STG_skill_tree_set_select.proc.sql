CREATE PROCEDURE [dbo].[STG_skill_tree_set_select]
	@iLoginUID_ [int]
AS
set nocount on

select
    CharType
,   Promotion
,   SetID
,   SlotID
,   SkillID
from dbo.STGSkillTreeSet
with (nolock)
where
    LoginUID = @iLoginUID_


