CREATE PROCEDURE [dbo].[CSGA_couple_system_select_check]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_select_check ( %d ) }

{ call dbo.CSGA_couple_system_select_check ( @1 ) }
@1 ; LoginUID int

n return ( @1 )
@1 ; LoginUIDE int ; 내가 커플 신청을 차단한 유저. -1은 전체임
*/

declare
    @sdtDateD smalldatetime
select
    @sdtDateD = convert(nvarchar(10), current_timestamp, 120)

begin transaction

delete a
with (updlock)
from dbo.CSGACoupleSystemCheck
as a
with (updlock)
where
    LoginUID = @iLoginUID_
and RegDateD <= dateadd(dd, 30, @sdtDateD)
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    LoginUIDE
from dbo.CSGACoupleSystemCheck
where
    LoginUID = @iLoginUID_
and RegDateD = @sdtDateD
option (maxdop 1)


