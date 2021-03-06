# w3daytim
w3daytim is simple enough 'daytime' client for Windows 3.1+Win32s.
Is it still a thing?

![w3daytim](https://user-images.githubusercontent.com/21245971/55373662-30e52880-5506-11e9-813d-2ff555dce514.png)

Anyway, you can use it to set the clock of your VM, 486 PC, microwave at 
boot time.

It expects the date format exposed by OpenBSD inetd internal 'daytime' 
server: "+%a %b %_d %H:%M:%S %Y"
As in: Tue Apr  2 04:18:51 2019

## Building
To build this, you need the following:
* GNU make
* mingw-w64:     Any mingw version should work, since we're targeting crtdll 
                 and that's been supported even since the early versions.
* InfoZIP's zip: That is, if you want to build 'dist' packages.
* GNU awk:       same reason. mawk also works
* A bit of love for really old and useless things

## Running
* Edit 'w3daytim.ini' to fit your needs
* Run the thing

## Obvious FAQ
**Q**: Why this program?

**A**: I might have too much free time. Also FFXIV servers were on maintenance.
##

**Q**: Why using the 'daytime' protocol? the 'date' one is more standard.

**A**: 2038
##

**Q**: Why not using the NTP protocol?

**A**: If you want to port libntp/ntpdate on Win32s/winsock 1.1, you're welcome!
##

**Q**: Why the gethostbyname()/gethostbyaddr() thing?

**A**: winsock 1.1
##

**Q**: Why this FAQ?

**A**: Check the first question
