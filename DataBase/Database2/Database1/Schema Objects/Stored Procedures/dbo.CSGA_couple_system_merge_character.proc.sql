CREATE PROCEDURE [dbo].[CSGA_couple_system_merge_character]
	@iLoginUID_ [int],
	@iCharType_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_merge_character ( %d, %d ) }

{ call dbo.CSGA_couple_system_merge_character ( @1, @2 ) }
@1 ; LoginUID int ; 내꺼
@2 ; CharType tinyint ; 선택된 캐릭터

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-2 ; 내 커플 정보가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.CSGACoupleSystem
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.CSGACoupleSystemCharacter
    where
        LoginUID = @iLoginUID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.CharType = @iCharType_
    from dbo.CSGACoupleSystemCharacter
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.CSGACoupleSystemCharacter
    with (updlock)
    (
        LoginUID
    ,   CharType
    )
    select
        @iLoginUID_
    ,   @iCharType_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

execute [Log].dbo.CSMO_couple_system_insert_character
    @iLoginUID_
,   @iCharType_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -103    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


