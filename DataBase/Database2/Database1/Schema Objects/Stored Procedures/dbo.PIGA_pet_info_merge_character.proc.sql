CREATE PROCEDURE [dbo].[PIGA_pet_info_merge_character]
	@iPetUID_ [int],
	@iCharType_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081203. microcat.

{ call dbo.PIGA_pet_info_merge_character ( %d, %d ) }

{ call dbo.PIGA_pet_info_merge_character ( @1, @2 ) }
@1 ; PetUID int
@2 ; CharType tinyint

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 펫 정보가 존재하지 않음
    -101이하 ; 트랜잭션 에러
*/

if not exists
(
    select *
    from dbo.GPet
    where
        PetUID = @iPetUID_
)
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.PIGAPetInfoCharacter
    where
        PetUID = @iPetUID_
    and CharType = @iCharType_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        Deleted = 0
    from dbo.PIGAPetInfoCharacter
    as a
    with (updlock)
    where
        PetUID = @iPetUID_
    and CharType = @iCharType_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.PIGAPetInfoCharacter
    (
        PetUID
    ,   CharType
    ,   Deleted
    )
    select
        @iPetUID_
    ,   @iCharType_
    ,   0

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


