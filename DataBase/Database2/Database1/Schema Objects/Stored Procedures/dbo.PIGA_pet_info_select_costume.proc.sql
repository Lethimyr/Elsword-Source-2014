CREATE PROCEDURE [dbo].[PIGA_pet_info_select_costume]
	@iPetUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081117. microcat.

{ call dbo.PIGA_pet_info_select_costume ( %d ) }

{ call dbo.PIGA_pet_info_select_costume ( @1 ) }
@1 ; PetUID int

1 return ( @1, @2 )
@1 ; CostumeUID int
@2 ; CostumeID int
     0         ; 기본 코스튬
     빈 데이터 ; 기본 코스튬
     기타      ; 해당 코스튬 아이템 ID
*/

select
    CostumeUID
,   CostumeID
from dbo.PIGAPetInfoCostume
where
    PetUID = @iPetUID_


