
Descrição passo a passo de como instalar a vps do NVA com Ubuntu 16.04 do zero para utilização do node-red

## Conteúdo

[1. Atualizar a VPS](#1-atualizar-a-VPS)  
[2. Instalar Curl  ](#2-Instalar-Curl)  
[3. Instalar nodejs V10](#3-Instalar-nodejs-V10)  
[4. Instalar Node-Red](#4-Instalar-Node-Red)  
[5. Executar node-red no boot](#5-Executar-node-red-no-boot)  
[6. Instalar o git](#6-Instalar-o-git)  
[7. Instalar packets node-red](#7-Instalar-packets-node-red)  
[8. Encaminhamento da home page](#8-Encaminhamento-da-home-page)  
[9. Criar repositório espelho do git hub](#9-Criar-repositório-espelho-do-git-hub)  
[10. Criação estrutua local do git](#10-Criação-estrutua-local-do-git)  
[Problema início automático ssh](#Problema-início-automático-ssh)  

### 1. Atualizar a VPS
```
    apt-get update
    apt-get install ca-certificates
    reboot
    sudo apt-get upgrade
```
<br>

### 2. Instalar Curl  

```
   sudo apt-get install curl
```

<br>

### 3. Instalar nodejs V10


```
   curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
   sudo apt-get install -y nodejs
```

 Verificar versão instalada

    node -v
        v10.16.0
 ```
 npm -v 
	6.9.0
```
<br>

### 4. Instalar Node-Red
```
	   sudo npm install -g --unsafe-perm node-red node-red-admin
```
<br>

### 5. Executar node-red no boot
 
Criar o arquivo  
	   sudo vi /etc/systemd/system/node-red.service  
Com as seguintes informações abaixo
 ```
[Unit]
Description=Node-RED
After=network.target

[Service]
Type=simple
#ExecStart=/usr/local/bin/node-red-pi --max-old-space-size=128 -v
ExecStart=/usr/bin/node-red 
Restart=on-failure
KillSignal=SIGINT


WorkingDirectory=/home/root
User=root

[Install]
WantedBy=multi-user.target 
```

Criar diretório
```
   mkdir /home/root
```
Executar os comandos
```
   sudo systemctl daemon-reload
   sudo systemctl enable node-red.service
   sudo systemctl start node-red.service
   sudo systemctl status node-red.service
```

Como Reinicilizar o serviço
```
   sudo systemctl restart node-red.service
```
<br>

### 6. Instalar o git
```
	apt-get install git-core
```
<br>

### 7. Instalar packets node-red

Parar o serviço do nodered para instalar os pacotes  
	sudo systemctl stop node-red.service

node-red-contrib-mqtt-broker 0.2.4
```
	npm install -g node-red-contrib-mqtt-broker
```
node-red-contrib-mdashboard 0.1.17
```
	npm install -g node-red-contrib-mdashboard
```

node-red-dashboard 2.15.5
```	
	npm install -g node-red-dashboard
```

node-red-node-pushover 0.0.15
```
	npm install -g node-red-node-pushover
```
node-red-node-darksky 0.1.18
```
	npm install -g node-red-node-darksky
```

<br>

### 8. Encaminhamento da home page
Arquivo index para encaminhamento da porta 80 para 1880/ui
```
cp /var/www/html/index.html /var/www/html/index.html.old
rm /var/www/html/index.html
vi /var/www/html/index.html
```
```	
	<!doctype html>
	<html>
	<head>
	<meta http-equiv="refresh" content="0;URL=http://iotpaulo.ddns.net:1880/ui">
	</head>
	<body>

	</body>
	</html>

```
<br>

### 9. Criar repositório espelho do git hub

Gerar a chave ssh e compartir no github
```
	ssh-keygen 
```

Copiar a chave em   
```
more /root/.ssh/id_rsa.pub  
```
Compartiar a Chave no Github  

 ![Compartiar a Chave no Github](https://i.imgur.com/SXYkFsR.jpg)


Criar repositório espelho do github e baixa arquivos

```
	mkdir /home/github
	cd /home/github	
	git init
	git config core.sparsecheckout true
	echo Server/ >> .git/info/sparse-checkout
	git remote add -f origin git@github.com:pauloeduardosp/Controle_de_Luz.git
        git remote add -f origin https://github.com/pauloeduardosp/Controle_de_Luz.git
	git pull origin master
```

mover os arquivos as respetivas pastas
arquivos weather
Criar repositório espelho do github e baixa arquivos
```
mkdir /home/img/
cp /home/github/Server/img_weather/*.* /home/img/
```
Arquivos node-red
```
cp /home/github/Server/flows_iotpaulo.json /root/.node-red
cp /home/github/Server/settings.js /root/.node-red
cp /home/github/Server/script_git.py /root/.node-red
cp /home/github/Server/lib/flows/*.json /root/.node-red/lib/flows/
```
<br>

### 10. Criação estrutua local do git

```
cd /root/.node-red
git init
git add lib/
git add script_git.py 
git add flows_iotpaulo.json
git add settings.js

echo .config.json >> .gitignore
echo .config.json.backup >> .gitignore
echo .flows_iotpaulo.json.backup >> .gitignore
echo .gitignore >> .gitignore
echo .sessions.json >> .gitignore
echo flows_iotpaulo_cred.json >> .gitignore
echo node_modules/ >> .gitignore
echo package-lock.json >> .gitignore
echo package.json >> .gitignore
```
### 11. Instalar EMQx

Baixar o EMQx
```
cd / && wget https://www.emqx.io/downloads/broker/v3.2.2/emqx-ubuntu16.04-v3.2.2.zip
```
<br>

Descompactar  
```
unzip emqx-ubuntu16.04-v3.2.2.zip
```
<br>


Apagar arquivo ZIP
```
rm emqx-ubuntu16.04-v3.2.2.zip
```
<br>


Iniciar EMQx no modo console para veriricar se tem algum erro
```
cd /emqx && ./bin/emqx console
```
<br>

Resultado
```
Starting emqx on node emqx@127.0.0.1
Start http:management listener on 8080 successfully.
Start http:dashboard listener on 18083 successfully.
Start mqtt:tcp listener on 127.0.0.1:11883 successfully.
Start mqtt:tcp listener on 0.0.0.0:1883 successfully.
Start mqtt:ws listener on 0.0.0.0:8083 successfully.
Start mqtt:ssl listener on 0.0.0.0:8883 successfully.
Start mqtt:wss listener on 0.0.0.0:8084 successfully.
EMQ X Broker 3.2.2 is running now!
```
sair console crtl+z
<br>

Iniciarliar EMQx	
```
cd /emqx && ./bin/emqx start
```
<br>

Dashboard 

http://x.x.x.x:18083
user: admin
pass: public


- Alterar senha de admin
-  ![Alterar senha de dmin](https://i.imgur.com/ep3n8ht.jpg)

- Inicializar serviço de autenticação Plugins/emqx_auth_username
- ![iniciarlizar Servico](https://i.imgur.com/jpfYSCU.jpg)

- Criar usuário mqtt - 
```
	cd /emqx && ./bin/emqx_ctl users add iotpaulo i0tp4ul0
```

-Mudar config para negar login anonymous
cd /emqx && vi /emqx/etc/emqx.conf

mudar parametro (linha 447 do arquivo)
dentro do vi usar : para pular para a liha
ou / para procurar o parametro "allow_anonymous"
allow_anonymous = false

./bin/emqx restart
<br><br>
### Problema início automático ssh
se tiver problema para ssh ao reiniciar a maquina
incluir as linhas no arquivo
```
vi /etc/rc.local
```
```
# Create the PrivSep empty dir if necessary
if [ ! -d /var/run/sshd ]; then
   mkdir /var/run/sshd
   chmod 0755 /var/run/sshd
fi
```

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEyNDg3NzE1NzUsLTIxMDU4MDQ4MSwtNz
g1NjUzMTc4LDg0OTc0Nzg4NiwtNzA1MTg5MjQyLC0xODE5NDUw
MzAyXX0=
-->