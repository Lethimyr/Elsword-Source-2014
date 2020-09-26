CREATE PROCEDURE [dbo].[STGA_skill_tree_merge_sp]
	@iLoginUID_ [int],
	@iCharType_ [tinyint],
	@iSPExp_ [int],
	@iSPLv_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090208. microcat.

L"{ call dbo.STGA_skill_tree_merge_sp ( %d, %d, %d, %d ) }"

{ call dbo.STGA_skill_tree_merge_sp ( @1, @2, @3, @4 ) }
@1 ; @iLoginUID_ [int],
@2 ; @iCharType_ [tinyint],
@3 ; @iSPExp_ [int],        ; 누적값임
@4 ; @iSPLv_ [tinyint],     ; 누적값임

1 return ( @1 )
@1 ; OK [int]
    0        ; 성공
    -1       ; 유저 정보가 존재하지 않음
    -101이하 ; 트랜잭션 에러
*/

if not exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if exists
(
    select *
    from dbo.STGSkillTreeSP
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        SPExp = @iSPExp_
    ,   SPLv = @iSPLv_
    from dbo.STGSkillTreeSP
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.STGSkillTreeSP
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   SPExp
    ,   SPLv
    )
    select
        @iLoginUID_
    ,   @iCharType_
    ,   @iSPExp_
    ,   @iSPLv_

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

insert into dbo.STLSkillTreeSP
with (updlock)
(
    LoginUID
,   CharType
,   SPExp
,   SPLv
,   RegDateB
)
select
    @iLoginUID_
,   @iCharType_
,   @iSPExp_
,   @iSPLv_
,   current_timestamp

if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


