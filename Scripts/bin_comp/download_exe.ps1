[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls, [Net.SecurityProtocolType]::Tls11, [Net.SecurityProtocolType]::Tls12, [Net.SecurityProtocolType]::Ssl3
Invoke-WebRequest 'https://mouzedrift.s-ul.eu/KTb8N0bc' -OutFile 10.5.exe
