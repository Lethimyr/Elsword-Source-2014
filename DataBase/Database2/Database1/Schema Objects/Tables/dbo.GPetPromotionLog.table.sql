CREATE TABLE [dbo].[GPetPromotionLog]
(
[PetUID] [int] NOT NULL,
[LoginUID] [int] NOT NULL,
[PetID] [int] NOT NULL,
[OldPetName] [nvarchar] (20) NOT NULL,
[NewPetName] [nvarchar] (20) NOT NULL,
[OldPromotion] [tinyint] NOT NULL,
[NewPromotion] [tinyint] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


