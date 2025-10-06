del sum.obj sum32.pdb sum32.exe
cl /Fe:sum32.exe sum.c /Zi /EHs-c- /GS- /Gw /Gy /GL /link /dynamicbase:no /opt:ref /subsystem:console
del sum.obj
