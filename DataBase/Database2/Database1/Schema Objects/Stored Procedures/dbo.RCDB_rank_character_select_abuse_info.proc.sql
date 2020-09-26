CREATE PROCEDURE [dbo].[RCDB_rank_character_select_abuse_info]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090115. microcat.
*/

declare
    @iLoginUID int

select
    @iLoginUID = LoginUID
from dbo.Users
where
    Login = @strLogin_

select
    convert(nvarchar(10), RegDate, 120) as RegDateD
,   Abused
from dbo.RCRRankCharacterAbuse
where
    LoginUID = @iLoginUID


