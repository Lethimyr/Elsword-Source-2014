CREATE PROCEDURE [dbo].[UGTE_user_gameinfo_select]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081115. microcat.

부연설명
이 sp는 2008년 11월 15일에 작성되었으며
약 1년 후에 이 sp는 삭제되어야 합니다.
아울러 dbo.Users.ModeLevel도 삭제되어야 합니다.

{ call dbo.UGTE_user_gameinfo_select ( %d ) }

{ call dbo.UGTE_user_gameinfo_select ( @1 ) }
@1 ; LoginUID int

1 return ( @1 )
@1 ; ModeLevel varbinary(200)
*/

select
    ModeLevel
from dbo.Users
where
    LoginUID = @iLoginUID_
option (maxdop 1)


