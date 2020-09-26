CREATE PROCEDURE [dbo].[up_ChaseQNA_Modify]
	@NO_ [int],
	@Subject_ [nvarchar](600),
	@Contents_ [text],
	@State_ [int],
	@OK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_ChaseQNA_Modify'

SET NOCOUNT ON

DECLARE @Str    NVARCHAR(3000)

        UPDATE  dbo.QuestionBoard SET Question = @Subject_, Anwer = @Contents_, state = @State_
        WHERE [NO] = @NO_

        

    IF @@Error <> 0 
        Begin
            SET @OK_ = 0
        End 
    Else
        Begin
            SET @OK_ = 1
        End


