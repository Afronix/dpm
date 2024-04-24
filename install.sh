#!/bin/bash

# Função para instalar o pacote dpkg
install_dpkg() {
    if command -v apt-get &>/dev/null; then
        sudo apt-get update
        sudo apt-get install -y dpkg
    elif command -v dnf &>/dev/null; then
        sudo dnf install -y dpkg
    elif command -v yum &>/dev/null; then
        sudo yum install -y dpkg
    else
        echo "Não foi possível determinar o gerenciador de pacotes instalado."
        exit 1
    fi
}

# Verifica se o dpkg está instalado
if ! command -v dpkg &>/dev/null; then
    install_dpkg
fi

# Verifica se o programa dpm existe em /bin/bash
if [ ! -x "/bin/dpm" ]; then
    # Dá permissão 777 ao programa dpm e copia para /bin/bash
    sudo chmod 777 dpm
    sudo cp dpm /bin/
    echo "dpm instalado com sucesso!!!"
    echo "Execute 'dpm --h' para saber como usar"
else
    echo "dpm já está instalado em sua máquina :)"
    echo "Execute: dpm --h"
fi
