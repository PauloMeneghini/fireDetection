Objetivo: Implementar um sistema de simulação de detecção de incêndios utilizando programação paralela e distribuída com pthreads e monitores para comunicação e sincronização entre as threads. A finalidade é desenvolver funções que utilizem monitores para realizar a comunicação segura entre diferentes threads que simulam nós sensores em uma matriz. É nescessário implementar uma thread que gera incêndios aleatórios periodicamente.

Descrição  
Imagine que você deve implementar um sistema de monitoramento de incêndios em uma floresta dividida em uma grade de células. Cada célula contém um nó sensor que monitora o estado da sua área. Esses nós comunicam entre si quando detectam incêndios, propagando mensagens aos vizinhos. Se um nó estiver localizado na borda da matriz, ele deve enviar a mensagem para uma thread central responsável por registrar e gerenciar os eventos de incêndio e iniciar um combate ao fogo. Além disso, o sistema deve incluir uma thread separada para gerar incêndios aleatórios.


Especificações do Sistema

    Tamanho da Floresta: Representar a floresta como uma matriz 30x30 com células que podem ter diferentes estados:
        -: área livre.
        T: célula monitorada por um nó sensor ativo.
        @: célula em chamas (fogo ativo).
        /: célula queimada.
    Nó Sensor: Cada nó deve ser representado por uma thread independente e se comunicar com seus vizinhos usando monitores. As funções de comunicação devem garantir exclusão mútua e comunicação segura utilizando mutexes e variáveis de condição.
    Thread Geradora de Incêndios: Uma thread adicional deve ser criada para gerar incêndios aleatórios em diferentes posições da matriz. A cada intervalo de tempo (e.g., 3 segundos), a thread deve selecionar uma célula - aleatória e marcar como fogo @.
    Central de Controle: Uma thread separada coleta mensagens dos nós de borda e age para combater incêndios.
    Combate ao Fogo: Implementar uma função para mudar o estado das células em chamas de @ para /.

