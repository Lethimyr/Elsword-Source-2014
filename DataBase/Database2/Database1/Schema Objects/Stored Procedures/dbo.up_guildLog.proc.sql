CREATE PROCEDURE [dbo].[up_guildLog]
	@GUBUN_ [nvarchar](1),
	@part_ [int],
	@Comment_ [nvarchar](2000),
	@beforegame_ [nvarchar](3),
	@aftergame_ [nvarchar](3),
	@afterclub_ [nvarchar](3),
	@gamertncode_ [nvarchar](3),
	@clubrtncode_ [nvarchar](3),
	@seq_ [int] OUTPUT,
	@return_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_guildlog'

SET NOCOUNT ON

if (@GUBUN_ = '0')
    begin
        INSERT INTO dbo.guild_log(part, comment, beforegame, aftergame, afterclub)
        VALUES(@part_, @comment_, @beforegame_, @aftergame_, @afterclub_)
    
        set @seq_ =  @@identity
        if @@error <> 0 
            GOTO Fail
    end 

else if (@GUBUN_ ='1')
    begin
        UPDATE dbo.guild_log SET aftergame = @aftergame_,gamertncode = @gamertncode_,comment = @comment_
        WHERE seq = @seq_

        if @@error <> 0 
            GOTO Fail

    end
else if (@GUBUN_ ='2')
    begin
        UPDATE dbo.guild_log SET afterclub = @afterclub_,clubrtncode = @clubrtncode_,comment = @comment_
        WHERE seq = @seq_

        if @@error <> 0 
            GOTO Fail

    end
GOTO succ


Fail:
    Set @return_ = -1

succ:
    Set @return_ = 0


