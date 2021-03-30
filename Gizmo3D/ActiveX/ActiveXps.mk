
ActiveXps.dll: dlldata.obj ActiveX_p.obj ActiveX_i.obj
	link /dll /out:ActiveXps.dll /def:ActiveXps.def /entry:DllMain dlldata.obj ActiveX_p.obj ActiveX_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ActiveXps.dll
	@del ActiveXps.lib
	@del ActiveXps.exp
	@del dlldata.obj
	@del ActiveX_p.obj
	@del ActiveX_i.obj
