CREATE PROCEDURE [dbo].[up_ChaseQNA_Write]
	@Subject_ [nvarchar](600),
	@Contents_ [text],
	@State_ [int],
	@OK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_ChaseQNA_Write'

SET NOCOUNT ON

Declare @Str nvarchar(3000)

		INSERT dbo.QuestionBoard(Question, Anwer, state, RegDate)
		Values(@SubJect_, @Contents_, @State_, getdate())

	IF @@Error <> 0 
		Begin
			SET @OK_ = 0
		End 
	Else
		Begin
			SET @OK_ = 1
		End


