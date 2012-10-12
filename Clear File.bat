@echo off
del *.sdf
del *.ipch /s
del dwcore*.exe /s
del *.log /s
del *.obj /s
del *.tlog /s
del *.pdb /s
del *.manifest /s
del *.lastbuildstate /s
del *.suo /q /f /a
del bin\*.dll
rd ipch /s /q
rd bin /s /q
rd VC100\dwcore__Win32_Release /s /q

pause >nul