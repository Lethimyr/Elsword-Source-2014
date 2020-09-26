CREATE PROCEDURE [dbo].[UGGA_user_gameclear_select]
	@iLoginUID_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081107. microcat.

{ call dbo.UGGA_user_gameclear_select ( %d ) }

{ call dbo.UGGA_user_gameclear_select ( @1 ) }
@1 ; LoginUID int

n return ( @1, @2 )
@1 ; ModeID smallint
@2 ; Difficulty smallint
*/

select
    ModeID
,   Difficulty
from dbo.UGGAUserGameClear
where
    LoginUID = @iLoginUID_
option (maxdop 1)


