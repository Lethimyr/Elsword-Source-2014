CREATE PROCEDURE [dbo].[CSGA_couple_system_select_message]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081111. microcat.

{ call dbo.CSGA_couple_system_select_message ( %d ) }

{ call dbo.CSGA_couple_system_select_message ( @1 ) }
@1 ; LoginUID int

1 return ( @1 )
@1 ; Message nvarchar(256)
*/

select
    Message
from dbo.CSGACoupleSystemMessage
where
    LoginUID = @iLoginUID_
option (maxdop 1)


