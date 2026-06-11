
Descrição passo a passo de como instalar a vps para utilização do node-red

## Conteúdo

[1. Atualizar a VPS](#1.-Atualizar-a-VPS) 
[2. Instalar o Node.js e o NPM - atualizado](#2.-Instalar-o-Node.js-e-o-NPM---atualizado)
[3. Instalar o Node-RED globalmente - atualizado](#3.-Instalar-o-Node-RED-globalmente---atualizado)
[4. Executar node-red no boot - atualizado](#4.-Executar-node-red-no-boot---atualizado)
&nbsp;&nbsp;&nbsp;&nbsp;[4.1 Executar outra instancia do node-red](#4.1-Executar-outra-instancia-do-node-red)
[6. Instalar o git](#6.-Instalar-o-git)
[7. Instalar packets node-red](#7.-Instalar-packets-node-red)
&nbsp;&nbsp;&nbsp;&nbsp;[7.1 Bkp do fluxo do node-red](#7.1-Bkp-do-fluxo-do-node-red)
&nbsp;&nbsp;&nbsp;&nbsp;[7.2 Forçar instalação dos pacotes faltantes - atualizado](#7.2-Forçar-instalação-dos-pacotes-faltantes---atualizado)
[8. Encaminhamento da home page](#8.-Encaminhamento-da-home-page)
[9. Criar repositório espelho do git hub](#9.-Criar-repositório-espelho-do-git-hub)
[10. Criação estrutua local do git](#10.-Criação-estrutua-local-do-git)
[11. Instalar EMQx - Atualizado](#11.-Instalar-EMQx---Atualizado)
[ Inicialiar EMQx no boot](#-Inicialiar-EMQx-no-boot)
[12. Instalar Mysql - atualizado](#12.-Instalar-Mysql---atualizado)
[Problema início automático ssh](#Problema-início-automático-ssh) 

### 1. Atualizar a VPS
```
    apt-get update
    apt-get install ca-certificates
    reboot
    sudo apt-get upgrade
```
<br>

### 2. Instalar o Node.js e o NPM - atualizado
O Node-RED precisa do Node.js para correr. Vamos instalar a versão estável atual diretamente dos repositórios do Ubuntu:
```
	sudo apt update
	sudo apt install nodejs npm -y
```
Para confirmar que foram bem instalados, verifique as versões com:
```
node -v
npm -v
```
<br>

### 3. Instalar o Node-RED globalmente - atualizado


```
   sudo npm install -g --unsafe-perm node-red
```

 Verificar versão instalada

    node -v
        v14.5.0
 ```
 npm -v 
	6.14.0
```
<br>

### 4. Executar node-red no boot - atualizado
 
Criar o arquivo  
 ```
	   sudo vi /etc/systemd/system/node-red.service  
```
Com as seguintes informações abaixo
```
[Unit]
Description=Node-RED
After=network.target

[Service]
Type=simple
User=root
Environment=NODE_OPTIONS="--max-old-space-size=512"
ExecStart=/usr/local/bin/node-red
Restart=on-failure
KillSignal=SIGINT

[Install]
WantedBy=multi-user.target 
```
Iniciar o Node-RED
```
	sudo systemctl daemon-reload
	sudo systemctl enable nodered.service
	sudo systemctl start nodered.service
```


<br>

### 4.1 Executar outra instancia do node-red
A pasta padrão da execuçaõ node red é    
```	/root/.node-red```

É necessário criar outra pasta para execução 
```
mkdir /root/.node-red2
```
Criar o arquivo um novo arquvo, com outro nome   
```	   
sudo vi /etc/systemd/system/node-red2.service  
```

Com as seguintes informações abaixo
 ```
[Unit]
Description=Node-RED2
After=network.target

[Service]
Type=simple
#ExecStart=/usr/local/bin/node-red-pi --max-old-space-size=128 -v
ExecStart=/usr/bin/node-red  -u /root/.node-red2 -p 1881     #pasta onde vai executar /root/.node-red2 e porta utilizada pelo serviço 1881

Restart=on-failure
KillSignal=SIGINT


WorkingDirectory=/home/root
User=root

[Install]
WantedBy=multi-user.target 
```

Executar os comandos
```
   sudo systemctl enable node-red2.service
   sudo systemctl start node-red2.service
   sudo systemctl status node-red2.service
```

Como Reinicilizar o serviço
```
   sudo systemctl restart node-red2.service
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

### 7.1 Bkp do fluxo do node-red
[Arquivo de bkp](BKP_Flow_Nodered).
<br>
### 7.2 Forçar instalação dos pacotes faltantes - atualizado
Caso tenha restauraurado o bkp, para instalar os pacotes faltantes
```
cd ~/.node-red
npm install --no-audit --no-fund
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
### 11. Instalar EMQx - Atualizado

Entrar na paguina
```
https://www.emqx.io/downloads/broker
```
E confirmar qual é a ultima versão do broker e ver o nome do arquivo para sua versão do Ubuntu

Baixar o EMQx
```
cd / && wget https://www.emqx.com/en/downloads/broker/vX.X.X/emqx-X.X.X-ubuntuY.Y-amd64.deb
```
<br>

Instalar
```
sudo apt install ./emqx-X.X.X-ubuntuY.Y-amd64.deb -y
```
<br>

Ativar e Iniciar o Serviço do EMQX
```
sudo systemctl enable emqx
sudo systemctl start emqx
```
<br>

Validar o serviço
```
sudo systemctl status emqx
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
<br>

-Mudar config para negar login anonymous
```
cd /emqx && vi /emqx/etc/emqx.conf
```
mudar parametro (linha 447 do arquivo) 
```
allow_anonymous = false
```
Dica - 
dentro do vi usar "**:**" para pular para a umero expecido da linha 
ou "**/**" para procurar o parametro "allow_anonymous"

Despois de editar o arquivo é necessário fazer um restart no serviço
./bin/emqx restart
#### Inicialiar EMQx no boot
É necessário incluir o serviço na crontab
```
crontab -e
```
e incluir a linha 
```
@reboot /emqx/bin/emqx start
```

<br><br>

### 12. Instalar Mysql - atualizado
```
sudo apt update sudo 
apt install mysql-server -y
```
Confimar status do serviço
```
sudo systemctl status mysql
```
```
Logar com usuário root
```
sudo mysql -u root -p
```
Mudar senha do root
```
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'joker98sp'
```
Criar banco e tabelas
```
CREATE DATABASE iotpaulo;

```
USE iotpaulo;
```
```
CREATE TABLE `Clima` ( `id` int unsigned NOT NULL AUTO_INCREMENT, `localidade` varchar(30) DEFAULT NULL, `sensor` varchar(30) DEFAULT NULL, `temperatura` float DEFAULT NULL, `umidade` float unsigned DEFAULT NULL, `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, PRIMARY KEY (`id`) ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```
```
CREATE TABLE `Offset` ( `id` int unsigned NOT NULL AUTO_INCREMENT, `localidade` varchar(30) DEFAULT NULL, `sensor` varchar(30) DEFAULT NULL, `tempoffset` float DEFAULT NULL, `umiddoffset` float unsigned DEFAULT NULL, `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, PRIMARY KEY (`id`) ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```
```
CREATE TABLE `Status` ( `id` int unsigned NOT NULL AUTO_INCREMENT, `localidade` varchar(30) NOT NULL, `status` float NOT NULL, `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, PRIMARY KEY (`id`) ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```
```
CREATE VIEW `view_Clima_Copare` AS select `a`.`id` AS `id_aberto`, `a`.`localidade` AS `loc_aberto`, `a`.`time` AS `time_aberto`, `a`.`temperatura` AS `temp_aberto`, `a`.`umidade` AS `umidd_aberto`, `f`.`id` AS `id_fechado`, `f`.`localidade` AS `loc_fechado`, `f`.`time` AS `time_fechado`, `f`.`temperatura` AS `temp_fechado`, `f`.`umidade` AS `umidd_fechado`, round((`f`.`temperatura` - `a`.`temperatura`),2) AS `dif_temp_fechado`, round((`f`.`umidade` - `a`.`umidade`),2) AS `dif_umidd_fechado`, `t`.`id` AS `id_termometro`, `t`.`localidade` AS `loc_termometro`, `t`.`time` AS `time_termometro`, `t`.`temperatura` AS `temp_termometro`, `t`.`umidade` AS `umidd_termometro`, round((`t`.`temperatura` - `a`.`temperatura`),2) AS `dif_temp_termometro`, round((`t`.`umidade` - `a`.`umidade`),2) AS `dif_umidd_termometro` from ((`Clima` `a` join lateral ( select `f`.`id` AS `id`,`f`.`localidade` AS `localidade`,`f`.`sensor` AS `sensor`,`f`.`temperatura` AS `temperatura`,`f`.`umidade` AS `umidade`,`f`.`time` AS `time` from `Clima` `f` where (`f`.`localidade` = 'fechado') order by abs(timestampdiff(SECOND,`f`.`time`,`a`.`time`)) limit 1 ) `f`) join lateral ( select `t`.`id` AS `id`,`t`.`localidade` AS `localidade`,`t`.`sensor` AS `sensor`,`t`.`temperatura` AS `temperatura`,`t`.`umidade` AS `umidade`,`t`.`time` AS `time` from `Clima` `t` where (`t`.`localidade` = 'termometro') order by abs(timestampdiff(SECOND,`t`.`time`,`a`.`time`)) limit 1 ) `t`) where ((`a`.`localidade` = 'aberto') and (abs(timestampdiff(SECOND,`f`.`time`,`a`.`time`)) <= 60) and (abs(timestampdiff(SECOND,`t`.`time`,`a`.`time`)) <= 60));
```

### Problema início automático ssh
se tiver problema para ssh ao reiniciar a maquina,
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
eyJoaXN0b3J5IjpbLTE5NzQwMTU5NjQsNzMyMjI0MDM2LC0xNz
MwNjMwMjM2LDE5NTMxODI3MTQsLTIxMDU4MDQ4MSwtNzg1NjUz
MTc4LDg0OTc0Nzg4NiwtNzA1MTg5MjQyLC0xODE5NDUwMzAyXX
0=
-->
