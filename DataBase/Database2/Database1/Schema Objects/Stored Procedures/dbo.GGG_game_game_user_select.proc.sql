CREATE PROCEDURE [dbo].[GGG_game_game_user_select]
	@iLoginUID_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080911. microcat.
*/

select
    ItemID
from dbo.GGGGameGameUser
where
    LoginUID = @iLoginUID_


