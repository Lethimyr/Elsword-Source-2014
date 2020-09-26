CREATE PROCEDURE [dbo].[up_myguildinfo]
	@strLogin_ [nvarchar](20),
	@iOK_ [int] OUTPUT,
	@Point_ [int] OUTPUT,
	@UserLevel_ [int] OUTPUT,
	@character_ [int] OUTPUT,
	@GuildID_ [int] OUTPUT,
	@Grade_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_myguildinfo'

SET NOCOUNT ON

DECLARE @Exp0_ bigint,
        @Exp1_ bigint,
        @Exp2_ bigint,
        @Exp3_ bigint,
        @Exp4_ bigint,
        @Exp5_ bigint,
        @Exp6_ bigint,
        @maxval bigint

-- 가입자의 존재 유무 확인
IF NOT EXISTS(SELECT Login FROM dbo.vw_GuildUser_List(nolock) WHERE Login=@strLogin_ )
    BEGIN
        SET @iOK_ = -1
        RETURN @iOK_
    END
Else
    BEGIN
        SELECT  @Exp0_ = EXP0, @EXP1_ = EXP1, @EXP2_ = EXP2,  @EXP3_ = EXP3,  @EXP4_ = EXP4,  @EXP5_ = EXP5,  @EXP6_ = EXP6, @Point_ = Point, @UserLevel_ = UserLevel ,@GuildId_ = GuildID,@Grade_ = Grade
        FROM dbo.vw_GuildUser_List(nolock)
        WHERE Login = @strLogin_
			
	Set @maxval = @Exp0_
	SET @Character_ = 0
			
            IF @maxval <  @Exp1_ 
             Begin
            	Set @maxval = @Exp1_
                SET @Character_ = 1
             End
             
            IF @maxval <  @Exp2_ 
             Begin
            	Set @maxval = @Exp2_
                SET @Character_ = 2
             End
                
            IF @maxval <  @Exp3_ 
             Begin
            	Set @maxval = @Exp3_
                SET @Character_ = 3
             End
             
            IF @maxval <  @Exp4_ 
             Begin
            	Set @maxval = @Exp4_
                SET @Character_ = 4
             End
             
            IF @maxval <  @Exp5_ 
             Begin
            	Set @maxval = @Exp5_
                SET @Character_ = 5
             End

            IF @maxval <  @Exp6_ 
             Begin
            	Set @maxval = @Exp6_
                SET @Character_ = 6
             End
             
             
        SET @iOK_ = 0
        RETURN @iOK_
        RETURN @Point_ 
        RETURN @UserLevel_ 
        RETURN @Character_ 
        RETURN @GuildID_ 
        RETURN @Grade_ 
        

    END


