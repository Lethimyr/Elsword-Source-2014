CREATE PROCEDURE [dbo].[MTM_manager_tool_character_info_query]
	@strLogin_ [nvarchar](20)
as
set nocount on

select
    CharType
,   Promotion
,   [Exp]
,   Level
,   Win
,   Lose
from dbo.Characters
with (nolock)
where
    Login = @strLogin_


