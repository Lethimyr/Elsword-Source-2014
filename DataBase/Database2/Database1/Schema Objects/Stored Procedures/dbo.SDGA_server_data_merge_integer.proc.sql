CREATE PROCEDURE [dbo].[SDGA_server_data_merge_integer]
	@sdtRegDateD_ [smalldatetime],
	@iType_ [tinyint],
	@iData_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090116. microcat.

L"{ call dbo.SDGA_server_data_merge_integer ( N'%s', %d, %d ) }"

{ call dbo.SDGA_server_data_merge_integer ( @1, @2, @3 ) }
@1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
@2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
@3 ; @iData_ [int]                  ; 차이값을 주쇼

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -101이하 ; 트랜잭션 에러
*/

declare
    @sdtRegDateD smalldatetime
select
    @sdtRegdateD = convert(nvarchar(10), @sdtRegDateD_, 120)

if exists
(
    select *
    from dbo.SDGAServerDataInteger
    where
        RegDateD = @sdtRegDateD
    and Type = @iType_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        Data = Data + @iData_
    from dbo.SDGAServerDataInteger
    as a
    with (updlock)
    where
        RegDateD = @sdtRegDateD
    and Type = @iType_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.SDGAServerDataInteger
    with (updlock)
    (
        RegDateD
    ,   Type
    ,   Data
    )
    select
        @sdtRegDateD
    ,   @iType_
    ,   @iData_

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


