CREATE PROCEDURE [dbo].[PIGA_pet_info_merge_costume]
	@iPetUID_ [int],
	@iCostumeUID_ [int],
	@iCostumeID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081117. microcat.

{ call dbo.PIGA_pet_info_merge_costume ( %d, %d, %d ) }

{ call dbo.PIGA_pet_info_merge_costume ( @1, @2, @3 ) }
@1 ; PetUID int
@2 ; CostumeUID_ int
@3 ; CostumeID_ int
     0         ; 기본 코스튬
     빈 데이터 ; 기본 코스튬
     기타      ; 해당 코스튬 아이템 ID

1 return ( @1 )
@1 ; OK int
     0        ; 성공
     -101이하 ; 트랜잭션 에러
*/

if exists
(
    select *
    from dbo.PIGAPetInfoCostume
    where
        PetUID = @iPetUID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        CostumeUID = @iCostumeUID_
    ,   CostumeID = @iCostumeID_
    from dbo.PIGAPetInfoCostume
    as a
    with (updlock)
    where
        PetUID = @iPetUID_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.PIGAPetInfoCostume
    with (updlock)
    (
        PetUID
    ,   CostumeUID
    ,   CostumeID
    )
    select
        @iPetUID_
    ,   @iCostumeUID_
    ,   @iCostumeID_

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


