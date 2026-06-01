# Guia: Extração de Tokens Xiaomi no Windows (Sem Admin)

Este guia demonstra como extrair o **Token** e a **BLE Bind Key (Encryption Key)** de dispositivos do ecossistema Mi Home/Xiaomi utilizando uma máquina Windows restrita, sem a necessidade de privilégios de administrador.

O método utiliza uma instalação local do Python (escopo do usuário) combinada com a ferramenta oficial da comunidade `Xiaomi-cloud-tokens-extractor`.

---

## 🛠️ Pré-requisitos

1. **Sensor pareado:** O dispositivo deve estar previamente configurado no aplicativo **Mi Home** do celular.
2. **Bluetooth Gateway:** Certifique-se de que o sensor já se comunicou ao menos uma vez com algum gateway Bluetooth da Xiaomi na sua casa (lâmpada, tomada, roteador inteligente, etc.) para que a chave de criptografia (`bindkey`) seja gerada na nuvem.

---

## 🚀 Passo a Passo no Windows

### Passo 1: Validar e Localizar o Python no Usuário
O Windows costuma ter o Python instalado de forma isolada dentro do perfil do usuário. Abra o **PowerShell** e certifique-se de que a pasta existe executando o comando abaixo:

```powershell
Test-Path "$env:USERPROFILE\AppData\Local\Programs\Python"


*Se o retorno for **True**, avance. Caso contrário, instale o Python através da Microsoft Store (que não exige senha de admin).*

### Passo 2: Baixar o Script Extrator Oficial
Ainda no PowerShell, execute o comando abaixo para baixar a versão mais recente do script criado por Piotr Machowski direto para a sua pasta de Downloads:

```powershell
Invoke-WebRequest -Uri "[https://raw.githubusercontent.com/PiotrMachowski/Xiaomi-cloud-tokens-extractor/master/token_extractor.py](https://raw.githubusercontent.com/PiotrMachowski/Xiaomi-cloud-tokens-extractor/master/token_extractor.py)" -OutFile "$env:USERPROFILE\Downloads\token_extractor.py"

```text
### Passo 3: Instalar as Dependências no Escopo do Usuário
O script necessita de bibliotecas de requisição, criptografia e tratamento de imagem (para exibição de Captchas no terminal). Instale-as apontando diretamente para o executável do Python do seu perfil:

```powershell
& (Get-ChildItem "$env:USERPROFILE\AppData\Local\Programs\Python\Python*\python.exe").FullName -m pip install requests pycryptodome Pillow```text


```text
### Passo 4: Executar a Ferramenta
Inicie o script interativo através do comando:

```powershell
& (Get-ChildItem "$env:USERPROFILE\AppData\Local\Programs\Python\Python*\python.exe").FullName "$env:USERPROFILE\Downloads\token_extractor.py"
