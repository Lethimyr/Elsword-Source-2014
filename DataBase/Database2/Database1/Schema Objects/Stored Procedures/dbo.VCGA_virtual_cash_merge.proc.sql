CREATE PROCEDURE [dbo].[VCGA_virtual_cash_merge]
	@iLoginUID_ [int],
	@iVCPointDiff_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090108. microcat.

L"{ call dbo.VCGA_virtual_cash_merge ( %d, %d ) }"

{ call dbo.VCGA_virtual_cash_merge ( @1, @2 ) }
@1 ; @iLoginUID_ int
@2 ; @iVCPointDiff_ int

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
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
    from dbo.VCGAVirtualCash
    where
        LoginUID = @iLoginUID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.VCPoint = a.VCPoint + @iVCPointDiff_
    from dbo.VCGAVirtualCash
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.VCGAVirtualCash
    with (updlock)
    (
        LoginUID
    ,   VCPoint
    )
    select
        @iLoginUID_
    ,   @iVCPointDiff_

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

execute [Log].dbo.VCMO_virtual_cash_insert
    @iLoginUID_
,   @iVCPointDiff_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -102    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


