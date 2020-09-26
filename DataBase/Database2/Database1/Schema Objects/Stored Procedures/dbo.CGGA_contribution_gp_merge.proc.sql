CREATE PROCEDURE [dbo].[CGGA_contribution_gp_merge]
	@iLoginUID_ [int],
	@iContributionGP_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081107. mk8253.

{ call dbo.CGGA_contribution_gp_merge ( %d, %d ) }

{ call dbo.CGGA_contribution_gp_merge ( @1, @2 ) }
@1 ; LoginUID int
@2 ; ContributionGP int

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 유저가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

IF NOT exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    END

IF EXISTS
(
    SELECT *
	FROM dbo.CGGAContributionGP
	WHERE
        LoginUID = @iLoginUID_
)
BEGIN
   BEGIN TRANSACTION
   
    UPDATE a
    with (updlock)
    SET Contribution = Contribution + @iContributionGP_
    FROM dbo.CGGAContributionGP AS a
    with (updlock)
    WHERE
        LoginUID = @iLoginUID_
        
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
END
ELSE
BEGIN
    BEGIN TRANSACTION
    
    INSERT INTO dbo.CGGAContributionGP
    with (updlock)
    (
        LoginUID
    ,   Contribution
    )
    SELECT
        @iLoginUID_
    ,   @iContributionGP_
    
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
END

execute [Log].dbo.CGMO_contribution_gp_insert
    @iLoginUID_  
,   @iContributionGP_  
,   @iOK output  
  
if @iOK <> 0  
begin    select @iOK = -104    goto fail_tran    end  

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


