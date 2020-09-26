CREATE PROCEDURE [dbo].[CSGA_couple_system_delete_equip]
	@iLoginUID_ [int],
	@iCharType_ [smallint],
	@iType_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_delete_equip ( %d, %d, %d ) }

{ call dbo.CSGA_couple_system_delete_equip ( @1, @2, @3 ) }
@1 ; LoginUID int ; 내꺼
@2 ; CharType smallint ; -1(유저), 0부터 엘리시스...
@3 ; Type tinyint ; 장착 아이템의 위치

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-2 ; 내 커플 정보가 존재하지 않음
-3 ; 내 장착 정보가 존재하지 않음
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

if not exists
(
    select *
    from dbo.CSGACoupleSystemEquip
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
    and Type = @iType_
)
begin    select @iOK = -3    goto end_proc    end


begin transaction

delete a
with (updlock)
from dbo.CSGACoupleSystemEquip
as a
with (updlock)
where
    LoginUID = @iLoginUID_
and CharType = @iCharType_
and Type = @iType_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


