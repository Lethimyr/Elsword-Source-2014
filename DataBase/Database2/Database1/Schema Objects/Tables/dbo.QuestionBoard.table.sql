CREATE TABLE [dbo].[QuestionBoard]
(
[no] [int] NOT NULL IDENTITY(1, 1),
[question] [varchar] (600) NOT NULL,
[anwer] [text] NOT NULL,
[regdate] [datetime] NOT NULL,
[state] [int] NOT NULL,
[bestflag] [char] (1) NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]


