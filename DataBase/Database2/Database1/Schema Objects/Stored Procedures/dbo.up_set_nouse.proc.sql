CREATE PROCEDURE [dbo].[up_set_nouse]
	@strSPname_ [nvarchar](50)
AS
exec dbo.up_log_set_proc_count 'up_set_nouse'

SET NOCOUNT ON

declare @strOldName nvarchar(50)
SET @strOldName = '_nouse_' + @strSPname_

if exists (select * from dbo.sysobjects(nolock) where id = object_id(N'[dbo].'+@strOldName) and OBJECTPROPERTY(id, N'IsProcedure') = 1)
exec up_set_nouse @strOldName

if exists (select * from dbo.sysobjects(nolock) where id = object_id(N'[dbo].'+@strSPname_) and OBJECTPROPERTY(id, N'IsProcedure') = 1)
exec sp_rename @strSPname_, @strOldName


