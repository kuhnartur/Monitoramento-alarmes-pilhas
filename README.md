# 🚨 Sistema de Monitoramento com Pilha de Alarmes

Trabalho acadêmico desenvolvido para a disciplina de **Estruturas de Dados** que consiste em uma implementação de um sistema de monitoramento de dispositivos IoT com pilha de alarmes em C.

## 📄 Descrição

O programa simula o monitoramento contínuo de **10 dispositivos** (sensores de temperatura e umidade). A cada ciclo, leituras aleatórias são geradas para cada dispositivo. Caso algum valor ultrapasse o limite aceitável, um alarme é registrado em uma **pilha circular de até 10 ocorrências** e também gravado em arquivo de log. O usuário pode consultar o último alarme ou todos os alarmes registrados, e o ciclo se repete indefinidamente com intervalo de 10 segundos.

## 📂 Estrutura do Projeto

```
.
├── Main.c #Código-fonte principal
├── Alarmes.txt #Arquivo de log gerado em tempo de execução
└── README.md #Documentação do Projeto
```


## ⚙️ Funcionalidades

- Geração aleatória de leituras de **temperatura** e **umidade** para 10 dispositivos
- Detecção de valores anormais, definido para acima de 50.0
- **Pilha de alarmes** tem a capacidade para até 10 ocorrências (Quando cheia, remove a ocorrência mais antiga e insere a nova no topo)
- Gravação de alarmes em arquivo de log `Alarmes.txt`
- Menu interativo a cada ciclo:
  - **Opção 1:** Exibe o alarme mais recente;
  - **Opção 2:** Exibe todos os alarmes (do mais recente ao mais antigo);
  - **Opção 3 (ou outro valor):** Continua sem exibir alarmes.
- Loop infinito com `sleep(10)` entre cada ciclo

## ▶️ Como Compilar e Executar

### Pré-requisitos

- GCC (GNU Compiler Collection)
- Sistema Linux/macOS (ou WSL no Windows)

### Compilação

```bash
gcc main.c -o monitoramento
```

### Execução

```bash
./monitoramento
```
> O arquivo `Alarmes.txt` será criado automaticamente no mesmo diretório ao detectar valores anormais.

## 📌 Algumas observações

- Os limites de alerta são fixos: **temperatura > 50°C** e **umidade > 50%**
- A pilha **não remove itens por ação do usuário** — apenas exibe
- O log em `Alarmes.txt` é aberto em modo **append** (`"a"`), preservando registros anteriores entre execuções


### Autor: [@kuhnartur](https://github.com/kuhnartur)