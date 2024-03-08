#!/bin/bash

## MONTADOR DOS SERVIDORES
## A SUA FUNÇÃO É UNIFICAR AS COMPILAÇÕES PARA NÃO TER QUE COMPILAR TODOS OS ARQUIVOS TODAS AS VEZES QUE FOR ATUALIZAR
## COMO UTILIZAR => bash assemble.sh

assembleServers() {
    echo "----------ASSEMBLE----------"
    echo "Limpando os executáveis..."
    make clean
    echo "Recompilando servidores..."
    make
    if [[ $? -ne 0 ]]; then
        echo "Ocorreu um erro ao remontar os servidores"
        exit 1
    fi
    echo "----------------------------"
    echo "Servidores montados com sucesso!"
    echo "Para executá-los, escreva:"
    echo "./server_executable [pt-BR | en-US]"
    echo "./client_executable"
    echo "Nessa exata ordem e em terminais diferentes."
}

assembleServers
