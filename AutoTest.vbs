set wshell=createobject("wscript.shell")
wshell.run "C:\projects\cpp-primer-5th-edition\Debug\CppPrimer5th.exe",1
wscript.sleep 1000
wshell.sendkeys "2"
wshell.sendkeys "{ENTER}"
wscript.sleep 300
wshell.sendkeys "1"
wshell.sendkeys "{ENTER}"
wscript.sleep 500
wshell.sendkeys "n"
wshell.sendkeys "{ENTER}"
wscript.sleep 500
wshell.sendkeys "y"
wshell.sendkeys "{ENTER}"
wscript.sleep 500
wshell.sendkeys "15"
wshell.sendkeys "{ENTER}"
wscript.sleep 300
wshell.sendkeys "4"
wshell.sendkeys "{ENTER}"
wscript.sleep 200
wshell.sendkeys "n"
wshell.sendkeys "{ENTER}"
wscript.sleep 200
wshell.sendkeys "n"
wshell.sendkeys "{ENTER}"
wscript.sleep 200
wshell.sendkeys "{ENTER}"