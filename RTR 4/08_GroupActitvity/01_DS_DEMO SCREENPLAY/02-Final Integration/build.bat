cls

del OGLMain.exe

del OGLMain.obj

del OGLMain.res

cl.exe /c /EHsc /D UNICODE /I "C:\Program Files (x86)\OpenAL 1.1 SDK\include" .\Models\Common.cpp  .\Models\OGLSCENE1.cpp .\Models\OGLSCENE2.cpp .\Models\OGLSCENE3.cpp OGLMain.cpp 

rc.exe OGLMain.rc 
rc.exe .\Models\OGLSCENE1.rc
rc.exe .\Models\OGLSCENE2.rc
rc.exe .\Models\OGLSCENE3.rc

link.exe OGLMain.obj Common.obj OGLSCENE1.obj OGLSCENE2.obj OGLSCENE3.obj  OGLMain.res .\Models\OGLSCENE1.res .\Models\OGLSCENE2.res .\Models\OGLSCENE3.res user32.lib gdi32.lib /LIBPATH:"C:\Program Files (x86)\OpenAL 1.1 SDK\libs\Win64" /SUBSYSTEM:WINDOWS

OGLMain.exe
