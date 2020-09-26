CREATE PROCEDURE [dbo].[PIGA_pet_info_select_character]
	@iPetUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081203. microcat.

{ call dbo.PIGA_pet_info_select_character ( %d ) }

{ call dbo.PIGA_pet_info_select_character ( @1 ) }
@1 ; PetUID int

n return ( @1 )
@1 ; CharType tinyint
*/

select
    CharType
from dbo.PIGAPetInfoCharacter
where
    PetUID = @iPetUID_
and Deleted = 0


