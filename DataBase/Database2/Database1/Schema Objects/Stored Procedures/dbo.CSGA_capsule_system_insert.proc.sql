CREATE PROCEDURE [dbo].[CSGA_capsule_system_insert]
	@iLoginUID_ [int],
	@iCharType_ [tinyint],
	@iKind_ [tinyint],
	@iLevel_ [tinyint],
	@iItemID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090224. microcat.

L"{ call dbo.CSGA_capsule_system_insert ( %d, %d, %d, %d, %d ) }"

{ call dbo.CSGA_capsule_system_insert ( @1, @2, @3, @4, @5 ) }
@1 ; @iLoginUID_ [int],
@2 ; @iCharType_ [tinyint],
@3 ; @iKind_ [tinyint],
@4 ; @iLevel_ [tinyint],
@4 ; @iItemID_ [int],

1 return ( @1 )
@1 ; OK int
    0         ; 성공
    -101 이하 ; 트랜잭션 에러
*/

begin transaction

insert into [Log].dbo.CSLOCapsuleSystem
with (updlock)
(
    LoginUID
,   RegDateA
,   CharType
,   Kind
,   Level
,   ItemID
)
select
    @iLoginUID_
,   current_timestamp
,   @iCharType_
,   @iKind_
,   @iLevel_
,   @iItemID_

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


