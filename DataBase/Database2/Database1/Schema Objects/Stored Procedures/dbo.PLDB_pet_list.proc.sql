CREATE PROCEDURE [dbo].[PLDB_pet_list]
AS
set nocount on

SELECT PetID, Promotion, Name as PetName, b.ItemID as PetItem, Factor
FROM dbo.GMessageName as a
WITH (nolock)
join dbo.GPetItem as b
with (nolock)
on a.ItemID = b.PetID
and a.offset = b.Promotion
where a.MessageID = 2
order by PetID, Promotion
OPTION (maxdop 1)


