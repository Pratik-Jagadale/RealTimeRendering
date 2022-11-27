cls

del OGL.exe

del OGL.obj

del OGL.res

rc.exe OGL.rc

nvcc.exe -I "C:\glew\include" -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\include" -L "C:\glew\lib\Release\x64" -L "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\lib\x64" user32.lib gdi32.lib OGL.res OGL.cu
