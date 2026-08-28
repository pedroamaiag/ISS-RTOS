# ISS-RTOS

Projeto acadêmico que acompanha a posição da Estação Espacial Internacional
e envia um aviso para um Arduino quando a ISS muda de cidade.

A aplicação foi feita em Python e o firmware foi desenvolvido
para Arduino Mega 2560.

## Como funciona

O programa em Python consulta a posição atual da ISS
e tenta identificar a cidade mais próxima.

Quando a cidade muda, o programa envia o caractere `A`
pela comunicação serial.

O Arduino recebe esse sinal e usa o kernel do projeto
para controlar um LED de aviso.

## Projeto

O repositório está dividido em duas partes principais:

* `host/`: aplicação em Python;
* `firmware/`: código do Arduino Mega 2560.

A documentação mais detalhada do projeto fica em:

```text
docs/
```

## Executar a aplicação

Crie um ambiente virtual:

```bash
python3 -m venv .venv
source .venv/bin/activate
```

Instale as dependências:

```bash
pip install -r requirements.txt
```

Execute:

```bash
python host/app.py
```

## Compilar o firmware

Instale o suporte ao Arduino AVR:

```bash
arduino-cli core update-index
arduino-cli core install arduino:avr
```

Compile para Arduino Mega 2560:

```bash
arduino-cli compile --fqbn arduino:avr:mega firmware/
```

## Documentação

Mais detalhes sobre o funcionamento do projeto podem ser encontrados em:

* [`docs/architecture.md`](docs/architecture.md)
* [`docs/kernel.md`](docs/kernel.md)
* [`docs/firmware.md`](docs/firmware.md)
* [`docs/host.md`](docs/host.md)
* [`docs/communication.md`](docs/communication.md)
* [`docs/setup.md`](docs/setup.md)

## Sobre

Este projeto foi desenvolvido originalmente como trabalho acadêmico
para estudar sistemas de tempo real e comunicação com microcontroladores.

O Arduino utilizado originalmente era virtual, através do Wokwi.
O projeto está sendo atualizado para também permitir o uso de um Arduino físico.
