cls
del OGL.exe 
del OGL.res
del OGL.obj

cl.exe /c /EHsc OGL.cpp

rc.exe OGL.rc

link.exe OGL.obj OGL.res gdi32.lib kernel32.lib user32.lib /SUBSYSTEM:WINDOWS

