CREATE PROCEDURE [dbo].[STGA_skill_tree_merge_set_equip]
	@iLoginUID_ [int],
	@iCharType_ [tinyint],
	@iPromotion_ [tinyint],
	@iSetID_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081114. microcat.

{ call dbo.STGA_skill_tree_update_set_equip ( %d, %d, %d, %d ) }

{ call dbo.STGA_skill_tree_update_set_equip ( @1, @2, @3, @4 ) }
@1 ; LoginUID int
@2 ; CharType tinyint
@3 ; Promotion tinyint
@4 ; SetID tinyint

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 유저 정보가 존재하지 않음
-2 ; 스킬 정보가 존재하지 않음
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

if not exists
(
    select *
    from dbo.STGSkillTreeSP
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.STGSkillTreeSetEquip
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
    and Promotion = @iPromotion_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.SetID = @iSetID_
    from dbo.STGSkillTreeSetEquip
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
        and CharType = @iCharType_
        and Promotion = @iPromotion_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.STGSkillTreeSetEquip
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   Promotion
    ,   SetID
    )
    select
        @iLoginUID_
    ,   @iCharType_
    ,   @iPromotion_
    ,   @iSetID_
    option (maxdop 1)

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


