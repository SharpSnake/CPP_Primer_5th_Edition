set wshell=createobject("wscript.shell")
wshell.run ".\Debug\CppPrimer5th.exe",1
wscript.sleep 100
wshell.sendkeys "2"
wshell.sendkeys "{ENTER}"
wscript.sleep 300
wshell.sendkeys "1"
wshell.sendkeys "{ENTER}"
wscript.sleep 500
wshell.sendkeys "n"
wshell.sendkeys "{ENTER}"
wscript.sleep 200
wshell.sendkeys "n"
wshell.sendkeys "{ENTER}"
wscript.sleep 200
wshell.sendkeys "{ENTER}"
