CREATE PROCEDURE [dbo].[gup_user_auth_value_set]
	@strLogin_ [nvarchar](20),
	@iValue_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 5월 30일
사용 : 유저 인증값을 저장하자
*/
set nocount on;

declare @iLoginUID int
select  @iLoginUID = LoginUID
    from dbo.Users with (nolock)
        where Login = @strLogin_
if @iLoginUID is null
    begin    select @iOK = -1    goto end_proc    end

declare @bNew bit
select @bNew = 1
if exists(select * from dbo.GUserAuthValue with (nolock) where LoginUID = @iLoginUID)
    begin    select @bNew = 0    end

begin tran
    if @bNew = 0
        begin
            update dbo.GUserAuthValue with (rowlock)
                set Value = @iValue_
                    where LoginUID = @iLoginUID
            if @@error <> 0 or @@rowcount <> 1
                begin    select @iOK = -11    goto fail_tran    end        
        end
    else if @bNew = 1
        begin
            insert into dbo.GUserAuthValue
                    (
                        LoginUID
,                       Value
                    )
                select  @iLoginUID
,                       @iValue_
            if @@error <> 0 or @@rowcount <> 1
                begin    select @iOK = -12    goto fail_tran    end        
        end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select  @iOK


