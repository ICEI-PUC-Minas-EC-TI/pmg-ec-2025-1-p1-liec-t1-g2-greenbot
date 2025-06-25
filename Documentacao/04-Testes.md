# Testes do Projeto
Os testes foram realizados de forma modular e, posteriormente, de maneira integrada, com foco nos seguintes aspectos:

Sensores Individuais:

Cada sensor (umidade do solo, temperatura, luminosidade e nível de água) foi testado separadamente. Foram utilizadas condições controladas, como solo seco e úmido, ambientes com diferentes intensidades de luz, e variação de temperatura ambiente para verificar se os sensores respondiam corretamente.

Funcionamento do Rosto no Display LCD 2.4” TFT (240x320, SPI):

Foram programadas diferentes expressões faciais para o "rosto do vaso", associadas aos estados da planta. O display LCD 2.4” TFT, com resolução de 240x320 e comunicação via SPI, foi configurado para exibir corretamente as imagens e animações das emoções (feliz, triste, alerta) conforme as leituras dos sensores. Testes foram realizados para garantir que as expressões fossem exibidas com nitidez, cores adequadas e atualização rápida, proporcionando uma interface visual intuitiva para o usuário.Descreva aqui como foram realizados os testes para o funcionamento do projeto. Descreva também os resultados encontrados e as limitações do projeto.

Comunicação entre ESP32 e Aplicativo:

Testou-se a conexão via Bluetooth, simulando o envio e recebimento de dados para verificar se a comunicação entre o ESP32 e o aplicativo ocorria corretamente. Foram realizados testes com variações na distância e interferências para avaliar a estabilidade e a confiabilidade da conexão Bluetooth, garantindo que os dados fossem transmitidos de forma estável e sem perdas.

Sistema de Irrigação Automatizado:

Foram realizados testes com diferentes níveis de umidade no solo para verificar se o acionamento da bomba d’água, controlada pelo módulo relé, ocorria no momento correto. Também foi testada a interação com o sensor de nível de água tipo boia para evitar acionamento sem água no reservatório.

Testes de Integração Geral:

Todo o sistema foi ligado e monitorado por períodos prolongados para simular condições reais de uso. Foram verificadas a sincronização entre sensores, o tempo de resposta do app, o comportamento do display e o acionamento da bomba.

Os testes demonstraram que o sistema funciona conforme o esperado nas seguintes situações:

Os sensores responderam adequadamente às variações ambientais, com leituras estáveis e compatíveis com os valores reais observados.

O display LCD 2.4” TFT apresentou corretamente as expressões faciais programadas, promovendo uma interface visual clara, colorida e funcional.

A comunicação entre o ESP32 e o aplicativo via Bluetooth foi eficiente, com transmissão estável e atualizações de dados em tempo real no app.

A bomba de irrigação foi acionada somente quando o solo estava seco e havia água no reservatório, comprovando a lógica correta de controle com o sensor de boia.

O aplicativo demonstrou boa usabilidade, notificando o usuário sempre que algum cuidado era necessário.
