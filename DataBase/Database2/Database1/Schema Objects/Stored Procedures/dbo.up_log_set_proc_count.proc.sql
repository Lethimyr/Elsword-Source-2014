CREATE PROCEDURE [dbo].[up_log_set_proc_count]
	@strProcName_ [nvarchar](50)
AS
SET NOCOUNT ON

--IF GetDate() > '2005-11-07 09:52'
--BEGIN
    RETURN
--END

IF objectproperty(object_id('LogProcCount'), 'TableIsPinned') = 0
BEGIN
    RETURN
END

IF NOT EXISTS(SELECT procName FROM dbo.LogProcCount(NOLOCK) WHERE procName = @strProcName_)
    BEGIN
        INSERT INTO dbo.LogProcCount(procName) Values(@strProcName_)
    END
RETURN


