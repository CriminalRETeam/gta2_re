[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest 'https://www.libsdl.org/release/SDL2-devel-2.0.22-VC.zip' -OutFile "SDL2-devel-2.0.22-VC.zip"
Invoke-WebRequest 'https://mouzedrift.s-ul.eu/KTb8N0bc' -OutFile 10.5.exe
