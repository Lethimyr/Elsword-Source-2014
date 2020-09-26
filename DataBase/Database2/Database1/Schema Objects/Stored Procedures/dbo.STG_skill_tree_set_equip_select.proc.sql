CREATE PROCEDURE [dbo].[STG_skill_tree_set_equip_select]
	@iLoginUID_ [int]
AS
set nocount on

select
    CharType
,   Promotion
,   SetID
from dbo.STGSkillTreeSetEquip
with (nolock)
where
    LoginUID = @iLoginUID_


