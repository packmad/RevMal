del sum.obj sum64.pdb sum64.exe
cl /Fe:sum64.exe sum.c /Zi /EHs-c- /GS- /Gw /Gy /GL /link /dynamicbase:no /opt:ref /subsystem:console
del sum.obj
