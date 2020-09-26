rem @echo off
:Loop
p4 sync //depot/X2Project/KncWX2Server/ServerPatchExe/LoginServer.*
..\ServerPatchEXE\LoginServer.exe NEXON_KOREA
goto Loop


