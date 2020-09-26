CREATE PROCEDURE [dbo].[STG_skill_tree_sp_select]
	@iLoginUID_ [int]
AS
set nocount on

select
    CharType
,   SPExp
,   SPLv
from dbo.STGSkillTreeSP
with (nolock)
where
    LoginUID = @iLoginUID_


