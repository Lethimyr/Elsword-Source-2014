CREATE PROCEDURE [dbo].[PIGA_pet_info_update_character]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081203. microcat.

{ call dbo.PIGA_pet_info_update_character ( %d ) }

{ call dbo.PIGA_pet_info_update_character ( @1 ) }
@1 ; LoginUID int

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -101이하 ; 트랜잭션 에러
*/

if exists
(
    select *
    from dbo.PIGAPetInfoCharacter
    as a
    join dbo.GPet
    as b
    on
        a.PetUID = b.PetUID
    where
        a.Deleted = 0
    and b.LoginUID = @iLoginUID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.Deleted = 1
    from dbo.PIGAPetInfoCharacter
    as a
    with (updlock)
    join dbo.GPet
    as b
    with (updlock)
    on
        a.PetUID = b.PetUID
    where
        a.Deleted = 0
    and b.LoginUID = @iLoginUID_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    commit transaction
end

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


