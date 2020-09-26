CREATE PROCEDURE [dbo].[beup_billing_error_count_update]
	@strErrorName_ [nvarchar](30),
	@iCount_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 10월 11일
사용 : 빌링 에러 코드 카운트 갱신
*/
set nocount on;

declare @bFirst bit
select  @bFirst = 1
if exists
    (
        select *
            from dbo.BEDataBillingErrorCount with (nolock)
                where ErrorName = @strErrorName_
    )
    begin
        select @bFirst = 0
    end

begin tran
    if @bFirst = 0
        begin
            update a with (updlock)
                set a.[Count] = a.[Count] + @iCount_
                    from dbo.BEDataBillingErrorCount as a with (updlock)
                        where a.ErrorName = @strErrorName_
            if @@error <> 0
                begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
        end
    else if @bFirst = 1
        begin
            insert into dbo.BEDataBillingErrorCount
                    (
                        ErrorName
,                       [Count]
                    )
                select  @strErrorName_
,                       @iCount_
            if @@error <> 0
                begin    select @iOK = -102, @iError = @@error    goto fail_tran    end
        end
commit tran

goto end_proc

fail_tran:
rollback tran

----
end_proc:
if @iOK <> 0
    begin
        declare @strDB nvarchar(4000)
,               @strProc nvarchar(4000)
        select  @strDB = db_name()
,               @strProc = object_name(@@procid)
        raiserror
            (
                'microcat : %s.dbo.%s ( OK : %d, Error : %d / ErrorName : %s, Count : %d )'
,               16, 127
,               @strDB, @strProc
,               @iOK, @iError
,               @strErrorName_
,               @iCount_
            )
    end
select @iOK


