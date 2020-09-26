CREATE PROCEDURE [dbo].[gup_pet_create]
	@iPetUID_ [int],
	@iLoginUID_ [int],
	@iPetID_ [int],
	@strPetName_ [nvarchar](20),
	@iCharType_ [smallint] = -1,
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*microcat
작성일 : 2006년 12월 28일
사용 : 펫 생성
-5(이미 존재하는 펫)
-6(캐릭터가 존재하지 않음)
-11이하(트랜잭션 에러)
*/

execute @iOK = dbo.gup_pet_check
    @iPetUID_
,   @iLoginUID_
,   @iPetID_
if @iOK <> 0
begin    goto end_proc    end

if exists
(
    select *
    from dbo.GPet
    where
        PetUID = @iPetUID_
    and LoginUID = @iLoginUID_
    and PetID = @iPetID_
)
begin    select @iOK = -5    goto end_proc    end

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_

if @strLogin is null
begin    select @iOK = -6    goto end_proc    end

if @iCharType_ <> -1
and not exists
(
    select *
    from dbo.Characters
    where
        Login = @strLogin
    and CharType = @iCharType_
)
begin    select @iOK = -6    goto end_proc    end

begin transaction

insert into dbo.GPet
with (updlock)
(
    PetUID
,   LoginUID
,   PetID
,   PetName
)
select
    @iPetUID_
,   @iLoginUID_
,   @iPetID_
,   @strPetName_

if @@error <> 0
begin    select @iOK = -11    goto fail_tran    end

insert into dbo.GPetExp
with (updlock)
(
    PetUID
,   Promotion
)
select
    @iPetUID_
,   Promotion
from dbo.GPetPromotion
with (updlock)

if @@error <> 0
begin    select @iOK = -12    goto fail_tran    end

if @iCharType_ <> -1
begin
    insert into dbo.PIGAPetInfoCharacter
    with (updlock)
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
    begin    select @iOK = -13    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
if @iOK = 0
begin
    select
        @iOK
    ,   Promotion
    ,   FullTime
    from dbo.GPet
    where
        PetUID = @iPetUID_
    and LoginUID = @iLoginUID_
    and PetID = @iPetID_
end
else
begin
    select
        @iOK
end


