
# Materiais

A montagem do hardware do Green Bot foi realizada com foco na integração entre sensores ambientais e um sistema de irrigação automatizado, utilizando os seguintes componentes principais:

ESP32 – Microcontrolador central do sistema devido à sua alta capacidade de processamento, memória e conectividade integrada, que inclui Wi-Fi e Bluetooth, permitindo comunicação eficiente com dispositivos externos e envio de dados ao aplicativo.

Sensor de Umidade do Solo Resistivo (Módulo) – Utilizado para medir a umidade presente no solo, com base na variação de resistência elétrica entre suas hastes. Esse módulo é fundamental para identificar quando a planta precisa ser regada, permitindo que o sistema reaja com expressões visuais apropriadas no display, facilitando a interação com o usuário.

Sensor de Temperatura e Umidade (DHT22) – Responsável por medir a temperatura e a umidade do ambiente, com boa precisão e estabilidade. Possui saída digital, o que facilita a integração com microcontroladores como o ESP32. Os dados fornecidos por este sensor contribuem para a análise das condições climáticas ao redor da planta, auxiliando na geração de expressões visuais adequadas no display, de acordo com o bem-estar da planta.

Sensor de Luminosidade (LDR) – Capta a intensidade da luz no ambiente onde a planta está localizada, ajudando a avaliar se ela recebe luz suficiente ao longo do dia.

Sensor de nível de Água tipo Boia – Utilizado no reservatório de água do sistema para detectar o nível mínimo de líquido. Quando a água está abaixo do nível crítico, o sensor impede a ativação da bomba, prevenindo o funcionamento a seco.

Mini Bomba Submersível (3-6V) – Responsável por bombear a água do reservatório para o vaso, permitindo a irrigação automática da planta conforme a leitura da umidade do solo.

Módulo Relé 1 Canal (5V) – Controla o acionamento da bomba de água a partir dos sinais enviados pelo ESP 32, garantindo que a irrigação ocorra apenas quando necessário.

Jumpers e fios diversos – Utilizados para realizar as interligações entre os componentes e o microcontrolador.

# Desenvolvimento

O desenvolvimento do projeto Green Bot foi dividido em duas frentes principais: aplicativo mobile e sistema embarcado (hardware). O processo foi conduzido de forma iterativa, com testes e ajustes em cada fase, visando à construção de uma solução funcional e integrada.

## Desenvolvimento do Aplicativo

### Interface

A interface do aplicativo foi desenvolvida na plataforma MIT App Inventor, escolhida por sua praticidade, ambiente visual amigável e facilidade de integração com sistemas embarcados como o ESP32. O layout foi projetado de forma simples e intuitiva, visando facilitar o uso por pessoas com diferentes níveis de familiaridade com a tecnologia.

O aplicativo conta com uma única tela funcional, na qual o usuário pode:

Visualizar as leituras em tempo real dos sensores de umidade do solo, temperatura e luminosidade;

Selecionar e conectar o dispositivo via Bluetooth por meio de uma lista de aparelhos disponíveis;

Acionar a irrigação manualmente através de um botão dedicado, quando necessário.

Essa abordagem concentrada em uma única interface garante maior simplicidade de navegação e torna o uso do sistema mais direto, prático e acessível, especialmente para usuários iniciantes ou com pouca familiaridade com aplicativos de monitoramento.

### Código

O código do aplicativo foi desenvolvido utilizando os blocos visuais do MIT App Inventor, que permitem a construção da lógica por meio de conexões tipo “arrastar e soltar” (drag and drop). Essa abordagem facilita a programação, mesmo para usuários com pouca experiência técnica.

As principais funcionalidades programadas foram:

Leitura dos dados via comunicação Bluetooth, recebendo em tempo real as informações transmitidas pelo microcontrolador (como umidade do solo, temperatura e luminosidade);

Atualização automática dos campos de leitura na interface do aplicativo, conforme os dados são recebidos do hardware;

Verificação dos valores recebidos por meio de blocos condicionais, que identificam situações críticas (ex: solo seco ou pouca luz) e alteram elementos visuais da interface para alertar o usuário;

Interface para seleção e conexão com o dispositivo Bluetooth, permitindo que o usuário reconecte o sistema em caso de perda de sinal.

## Desenvolvimento do Hardware

### Montagem

A montagem do projeto iniciou-se com o planejamento do esquema elétrico, definindo os componentes necessários e suas conexões físicas. O microcontrolador ESP32 foi escolhido como unidade central devido à sua conectividade integrada e capacidade de processamento.

Preparação dos Componentes

Foram reunidos todos os componentes eletrônicos: ESP32, sensores (temperatura, umidade, luminosidade, etc.), módulos Bluetooth (se necessário), fontes de alimentação, resistores, fios e protoboard ou placa de circuito impresso (PCB).

Montagem Física e Conexões

Com base no esquema elétrico, os sensores foram conectados aos pinos apropriados do ESP32, respeitando as especificações de alimentação e sinais. A comunicação Bluetooth do ESP32 foi configurada no próprio chip, dispensando módulos externos.

Configuração da Alimentação

A alimentação foi cuidadosamente organizada para garantir tensão estável e suficiente para todos os componentes, evitando ruídos ou falhas.

Montagem da Interface de Comunicação

Foi estabelecida a conexão Bluetooth entre o ESP32 e o aplicativo móvel, garantindo que o hardware estivesse apto a receber e enviar dados sem interferências.

Testes de Conexão e Funcionamento Inicial

Antes da montagem final, cada componente foi testado individualmente para verificar seu funcionamento. Posteriormente, testes integrados confirmaram a comunicação correta entre sensores, ESP32 e o app via Bluetooth.

Ajustes Finais e Validação

Após a montagem física, foram realizados testes completos do sistema, avaliando a estabilidade da comunicação Bluetooth, a precisão das leituras dos sensores e a resposta do aplicativo.

### Desenvolvimento do Código

O desenvolvimento do código para o Arduino/ESP foi realizado utilizando a linguagem C++, com foco na integração dos sensores e na comunicação com dispositivos externos, como aplicativos móveis.

Inicialmente, o código foi estruturado para configurar os pinos de entrada e saída do microcontrolador, e incluir as bibliotecas necessárias para leitura dos sensores e comunicação, como Bluetooth.

No método de inicialização (setup()), foram ativados os periféricos, configurada a comunicação serial para monitoramento e inicializado o módulo de comunicação (Bluetooth), preparando o sistema para troca de dados.

No loop principal (loop()), o código realiza a leitura dos sensores periodicamente, processa os dados obtidos e envia essas informações para o dispositivo externo conectado, garantindo atualização em tempo real.

Além disso, o sistema também recebe comandos ou ajustes do dispositivo remoto, permitindo controle e interatividade.

Durante o desenvolvimento, foram realizados testes e ajustes para garantir a estabilidade da comunicação, o correto funcionamento dos sensores e a robustez geral do sistema.

## Comunicação entre App e Hardware

A comunicação entre o aplicativo móvel e o Arduino/ESP foi estabelecida utilizando o protocolo Bluetooth, devido à sua praticidade e baixo consumo de energia para conexão em curtas distâncias.

No lado do Arduino/ESP, foi configurado o módulo Bluetooth (nativo no caso do ESP32, ou externo no Arduino) para atuar como servidor, ficando disponível para conexão com dispositivos Bluetooth próximos. O código implementado no microcontrolador realiza a inicialização do Bluetooth, define um nome identificador para o dispositivo e monitora constantemente a presença de conexões.

O aplicativo móvel, por sua vez, busca e identifica o dispositivo Bluetooth disponibilizado pelo Arduino/ESP, estabelecendo o pareamento e a conexão via Bluetooth. Essa conexão possibilita a troca bidirecional de dados entre os dois dispositivos.

Durante a operação, o Arduino/ESP coleta dados dos sensores conectados, processa essas informações e envia periodicamente os dados ao aplicativo via Bluetooth, em formatos simples como strings ou pacotes de dados estruturados. Isso permite que o aplicativo exiba as informações em tempo real para o usuário.

Além disso, o aplicativo pode enviar comandos ou parâmetros ao Arduino/ESP através da conexão Bluetooth, permitindo controlar dispositivos, ajustar configurações ou acionar funções específicas no hardware.

Para garantir a confiabilidade, o código do Arduino/ESP inclui rotinas para detectar desconexões e reconectar automaticamente, mantendo a comunicação ativa durante o uso.

O protocolo utilizado é simples e eficiente, garantindo baixa latência na transmissão dos dados e mantendo a sincronização entre o sistema embarcado e o aplicativo.
