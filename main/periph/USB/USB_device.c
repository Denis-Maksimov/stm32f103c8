/******************************************************* /
 * С этим интерфейсом, чувствую, что долго протрахаюсь.. /
 ********************************************************/
/*

  Вот по-Гугловски перевод референс-мануала 

//--- Universal serial bus full-speed device interface (USB) ---//

=== USB introduction ===

Периферийное устройство USB реализует интерфейс между 
full-speed шиной USB 2.0 и шиной APB1.
Поддерживается приостановка/возобновление USB, 
что позволяет останавливать тактирование(или часы) устройства 
при низком энергопотреблении.

=== USB main features ===

-Совместимость со спецификацией USB версии 2.0
-Конфигурируемое количество конечных точек от 1 до 8
- Генерация/проверка циклического избыточного кода (CRC), 
кодирование/декодирование без возврата к нулю (NRZI) и бит-стаффинг
- Поддержка изохронных транзакций
-Двойная буферизированная поддержка массовой/изохронной конечной точки
-USB-операции "приостановить"/"возобновить"
-Генерация тактовых импульсов с блокировкой кадров.
 
USB и CAN совместно используют выделенную 512-байтовую память SRAM для передачи и приема данных, 
поэтому их нельзя использовать одновременно (доступ к общей ОЗУ осуществляется через CAN и USB исключительно). 
USB и CAN могут использоваться в одном приложении, но не одновременно!!!


=== USB functional description ===

Периферийное устройство USB обеспечивает USB-совместимое соединение 
между ПК-хостом и функцией, реализуемой микроконтроллером. 
Передача данных между хост-ПК и системной памятью
происходит через выделенную буферную память пакетов, 
доступ к которой напрямую осуществляется через периферийное устройство USB.
Размер этой выделенной буферной памяти должен соответствовать 
количеству используемых конечных точек и максимальному размеру пакета. 
Эта выделенная память имеет размер до 512 байт и может использоваться 
до 16 однонаправленных или 8 двунаправленных конечных точек. 
Периферийное устройство USB взаимодействует с хостом USB, обнаруживая пакеты токенов, 
обрабатывая передачу / прием данных и обрабатывая пакеты рукопожатия, 
как того требует Стандарт USB. 
Форматирование транзакций выполняется аппаратными средствами, 
включая генерацию и проверку CRC.

Каждая конечная точка связана с блоком описания буфера, 
указывающим, где находится область памяти, связанная с конечной точкой, 
насколько она велика или сколько байтов должно быть передано. 
Когда периферийным устройством USB распознается токен для 
действительной пары функция/конечная точка, 
происходит соответствующая передача данных 
(если требуется и если конечная точка настроена). 
Данные, буферизованные периферийным устройством USB, 
загружаются во внутренний 16-битный регистр,
и осуществляется доступ к памяти в выделенном буфере. 
Когда все данные были переданы, при необходимости, 
генерируется или ожидается соответствующий пакет рукопожатия по USB
в соответствии с направлением передачи. 
В конце транзакции генерируется специфичное для конечной точки прерывание, 
считывающее регистры состояния и/или использующее 
различные процедуры ответа на прерывание. 

Микроконтроллер может определять: 
• обслуживаемую конечную точку 
• тип транзакции, которая произошла, если произошли ошибки 
(такие как вставка битов, формат, CRC, протокол, пропущенный ACK, переполнение/опустошение). 

Специальная поддержка предлагается для изохронных передач 
и массовой передачи с высокой пропускной способностью, 
реализующих использование двойного буфера, 
что позволяет всегда иметь доступный буфер для периферийного устройства USB, 
в то время как микроконтроллер использует другой. 
Устройство можно перевести в режим пониженного энергопотребления 
(режим SUSPEND), записав в контрольный регистр, когда это необходимо.
Благодаря этому уменьшается статическое потребление мощности, 
и тактирование USB можно замедлять или останавливать.
Обнаружение активности на входах USB в режиме пониженного энергопотребления
вызывает асинхронное включение устройства. 
Специальный источник прерывания может быть подключен непосредственно к линии пробуждения, 
чтобы позволить системе немедленно перезапустить нормальную генерацию тактовых импульсов 
и/или поддерживать прямой запуск/останов тактовых генераторов.

=== Description of USB blocks ===

Периферийное устройство USB реализует все функции, связанные с интерфейсом USB, 
которые включают в себя следующие блоки: 
............................................................
• Модуль последовательного интерфейса (SIE): 
    Функции этого блока включают в себя: 
    распознавание шаблона синхронизации, 
    вставку битов, 
    генерацию и проверку CRC, 
    проверку/генерацию PID 
    и оценка рукопожатия. 
Он должен взаимодействовать с приемопередатчиками USB и использовать 
виртуальные буферы, предоставляемые интерфейсом буфера пакетов для 
локального хранения данных. Это устройство также генерирует сигналы 
в соответствии с периферийными событиями USB, такими как 
начало кадра (SOF), USB_Reset, ошибки данных и т.д., И событиями, 
связанными с конечной точкой, такими как завершение передачи 
или правильный прием пакета; 
эти сигналы затем используются для генерации прерываний. 
............................................................
• Таймер: 
этот блок генерирует синхронизированный импульс начала кадра 
и обнаруживает глобальную приостановку (от хоста), 
когда трафик не получен в течение 3 мс. 
............................................................
• Интерфейс пакетного буфера: 
Это Блок управляет локальной памятью, реализуя набор буферов
гибким способом, как для передачи, так и для приема. 
Он может выбрать нужный буфер в соответствии с запросами, 
поступающими от SIE, и найти их в адресах памяти, 
указанных регистрами конечных точек. 
Он увеличивает адрес после каждого обмененного слова до конца пакета, 
отслеживая количество обмененных байтов и предотвращая переполнение 
максимальной ёмкости буфера. 
............................................................
• Регистры, связанные с конечной точкой: 
каждая конечная точка имеет связанный регистр, содержащий 
тип конечной точки и ее текущий статус. 
Для однонаправленных/однобуферных конечных точек один регистр 
может использоваться для реализации двух различных конечных точек. 
Количество регистров равно 8, что позволяет использовать 
до 16 однонаправленных/однобуферных или до 7 двойных буферов 
в любой комбинации. Например, периферийное устройство USB может 
быть запрограммировано так, чтобы оно имело 4 конечных точки 
с двумя буферами и 8 однобуферных/однонаправленных конечных точек.
............................................................
• Регистры управления: 
Это регистры, содержащие информацию о состоянии всей периферии USB 
и используемые для принудительного запуска некоторых событий USB, 
таких как возобновление и отключение питания. 
............................................................
• Регистры прерываний: 
они содержат маски прерываний и запись событий. 
Их можно использовать для выяснения причины прерывания, 
состояния прерывания или для очистки состояния ожидающего прерывания. 

____________________________________________________________
USB-периферия подключается к шине APB1 через интерфейс APB1, 
содержащий следующие блоки: 
____________________________________________________________


• Память пакетов: 
это локальная память, которая физически содержит буфер пакетов. 
Он может использоваться интерфейсом Packet Buffer, 
который создает структуру данных и может быть доступен 
непосредственно из прикладного программного обеспечения. 
Размер памяти пакета составляет 512 байт, структурированный как 256 слов 
на 16 бит. 
............................................................
• Арбитр: 
этот блок принимает запросы памяти, поступающие от шины APB1 
и от интерфейса USB. Он разрешает конфликты, отдавая приоритет 
доступам APB1, при этом всегда резервируя половину 
пропускной способности памяти для завершения всех передач USB. 
Эта схема временного дуплекса реализует виртуальную двухпортовую SRAM, 
которая обеспечивает доступ к памяти во время транзакции USB. 
Эта схема также допускает передачу нескольких слов APB1 любой длины. 
............................................................
• Преобразователь регистров:(Register Mapper) 
этот блок собирает различные регистры USB и периферийных устройств 
шириной в байт и в бит в структурированном наборе слов шириной 16 бит, 
адресованном APB1. 
............................................................
• APB1 Wrapper: (обёртка?)
предоставляет интерфейс для APB1 для памяти и регистрации. 
Он также отображает всю периферию USB в адресном пространстве APB1. 
............................................................
• Преобразователь прерываний: 
этот блок используется для выбора того, как возможные события USB 
могут генерировать прерывания, и сопоставления их с тремя различными 
строками NVIC: 

    – USB-прерывание с низким приоритетом (канал 20): 
    инициируется всеми событиями USB (правильная передача, 
    сброс USB и т. Д.). Микропрограмма должна проверить 
    источник прерывания перед обработкой прерывания. 

    – Прерывание с высоким приоритетом USB (канал 19): 
    инициируется только правильным событием передачи 
    для изохронной и двойной буферизации массовой передачи 
    для достижения максимально возможной скорости передачи. 

    – USB wakeup Прерывание (Канал 42): 
    Инициируется событием пробуждения из режима USB Suspend.

............................................................


=== Programming considerations ===
//!-------Инструкция о том, как ломать-----------------
    :::Вступление:::
=== Generic USB device programming ===

    В этой части описываются основные задачи, 
    которые требуются прикладному программному обеспечению 
    для обеспечения совместимости с USB.
    Действия, связанные с наиболее общими событиями USB, 
    принимаются во внимание, а параграфы посвящены особым 
    случаям конечных точек с двойной буферизацией и изохронной передачи. 

    Помимо сброса системы, действие всегда инициируется периферийным 
    устройством USB, управляемым одним из событий USB, описанных ниже.

//!### System and power-on reset ###
    После сброса системы и включения питания первой операцией, 
    которую должно выполнить прикладное программное обеспечение, 
    является подача всех необходимых тактовых сигналов 
    на периферийное устройство USB и последующая де-активация 
    его сигнала сброса, чтобы иметь возможность доступа к его регистрам. 
    Вся последовательность инициализации описана ниже. 

    В качестве первого шага прикладному программному обеспечению 
    необходимо активировать тактовый регистр макроячейки 
    и деактивировать специальный сигнал сброса макроячейки, 
    используя соответствующие биты управления, 
    предоставленные логикой управления тактовым импульсом устройства. 
    После этого аналоговая часть устройства, связанная с приемопередатчиком USB, 
    должна быть включена с помощью бита PDWN в регистре CNTR, 
    что требует специальной обработки. 
    Этот бит предназначен для включения внутренних опорных напряжений, 
    которые питают порт приемопередатчика. 
    Эта схема имеет определенное время запуска (tSTARTUP, указанное 
    в таблице данных), в течение которого поведение USB-трансивера 
    не определено. Таким образом, необходимо подождать это время 
    после установки бита PDWN в регистре CNTR, прежде чем удалять условие 
    сброса на USB-части (путем сброса бита FRES в регистре CNTR).
    ??? Очистка регистра ISTR затем удаляет любое ложное ожидающее прерывание, 
    ??? прежде чем любая другая операция макросоты будет включена.

    При перезагрузке системы микроконтроллер должен инициализировать 
    все необходимые регистры и таблицу описания буфера пакетов, 
    чтобы периферийное устройство USB могло правильно генерировать 
    прерывания и передачу данных. Все регистры, не относящиеся 
    к какой-либо конечной точке, должны быть инициализированы 
    в соответствии с потребностями прикладного программного 
    обеспечения (выбор разрешенных прерываний, 
    выбранный адрес буферов пакетов и т. Д.). Затем процесс продолжается, 
    как в случае сброса USB (см. Дальнейший параграф)

//!### USB reset (RESET interrupt) ###

    Когда это событие происходит, периферийное устройство USB переводится 
    в то же состояние, в которых оно было сброшено системой 
    после инициализации, описанной в предыдущем параграфе: 
    связь отключена во всех регистрах конечных точек 
    (периферийное устройство USB не будет отвечать ни на один пакет). 
    В ответ на событие сброса USB должна быть включена функция USB, 
    имеющая в качестве адреса USB "0" реализацию только конечной точки 
    управления по умолчанию (адрес конечной точки тоже равен 0). 
    Это достигается установкой бита функции активации (EF) 
    регистра USB_DADDR и инициализацией регистра EP0R и связанных 
    с ним буферов пакетов соответственно. 
    Во время процесса перечисления USB хост назначает этому устройству 
    уникальный адрес, который должен быть записан в битах ADD [6: 0] 
    регистра USB_DADDR, и настраивает любую другую необходимую конечную точку. 
    При получении прерывания RESET прикладное программное 
    обеспечение отвечает за повторное включение конечной точки 
    по умолчанию функции USB 0 в течение 10 мс после окончания 
    последовательности сброса, которая вызвала прерывание.

//!###Структура и использование буферов пакетов (Structure and usage of packet buffers) ###

    Каждая двунаправленная конечная точка может принимать или передавать данные 
    с/на хост. Полученные данные хранятся в выделенном буфере памяти, 
    зарезервированном для этой конечной точки, тогда как другой буфер 
    памяти содержит данные, которые должны быть переданы конечной точкой. 
    Доступ к этой памяти осуществляется блоком интерфейса пакетного буфера, 
    который доставляет запрос доступа к памяти и ожидает его подтверждения. 
    Поскольку микроконтроллер также должен осуществлять доступ к памяти буфера 
    пакетов, логика арбитража учитывает конфликты доступа, 
    используя половину цикла APB1 для доступа к микроконтроллеру 
    и оставшуюся половину для доступа к периферийному устройству USB. 
    Таким образом, оба агента могут работать так, как если бы пакетная 
    память представляла собой двухпортовую SRAM, не зная о каком-либо конфликте, 
    даже когда микроконтроллер выполняет параллельный доступ. 
    Периферийная логика USB использует выделенные часы. 
    Частота этих выделенных часов фиксируется требованиями стандарта USB на 48 МГц, 
    и это может отличаться от часов, используемых для интерфейса с шиной APB1. 
    Возможны разные конфигурации часов, в которых тактовая частота 
    APB1 может быть выше или ниже, чем у периферийного USB.

    //!Примечание: 
    //!   Из-за требований к скорости передачи данных USB и интерфейсу пакетной 
    //!   памяти тактовая частота APB1 должна быть выше 8 МГц, 
    //!   чтобы избежать проблем переполнения / переполнения данных.

    Каждая конечная точка связана с двумя буферами пакетов 
    (обычно один для передачи, а другой для приема). 
    Буферы могут быть размещены в любом месте внутри пакетной памяти, 
    поскольку их местоположение и размер указываются в таблице описания буфера, 
    которая также находится в пакетной памяти по адресу, 
    указанному в регистре USB_BTABLE. 
    Каждая запись таблицы связана с регистром конечной точки 
    и состоит из четырех 16-битных слов, так что начальный адрес 
    таблицы всегда должен быть выровнен по 8-байтовой границе 
    (младшие три бита регистра USB_BTABLE всегда равны «000»). 
    Записи таблицы дескрипторов буфера описаны в разделе 23.5.3. 
    Если конечная точка является однонаправленной и не является 
    изохронной или с двойной буферизацией, требуется только 
    один буфер пакетов (тот, который связан с поддерживаемым направлением передачи). 
    Другие местоположения таблицы, связанные с неподдерживаемыми 
    направлениями передачи или неиспользованными конечными точками, 
    доступны пользователю. 
    Массовые конечные точки с изохронной и двойной буферизацией 
    имеют специальную обработку буферов пакетов 
    (см. Соответственно раздел 23.4.4 и раздел 23.4.3). 
    Связь между записями таблицы описания буфера и областями буфера пакетов 
    подробно показана на рисунке 221.
    Каждый пакетный буфер используется во время приема или передачи, 
    начиная снизу. Периферийное устройство USB никогда не изменит 
    содержимое областей памяти, смежных с выделенными буферами памяти; 
    если получен пакет, превышающий выделенную длину буфера 
    (условие переполнения буфера), данные будут скопированы в память 
    только до последнего доступного местоположения.

//!### Инициализация конечной точки (Endpoint initialization) ###

    1)Первым шагом для инициализации конечной точки является запись 
    соответствующих значений в регистры ADDRn_TX/ADDRn_RX, 
    чтобы периферийное устройство USB обнаружило, 
    что передаваемые данные уже доступны, а принимаемые данные могут 
    быть буферизованы. 

    2)Биты EP_TYPE в регистре USB_EPnR должны быть установлены в соответствии 
    с типом конечной точки, 

    3)в конечном итоге используя бит EP_KIND для включения любой специальной требуемой функции. 

    4)На передающей стороне конечная точка должна быть активирована 
    с использованием битов STAT_TX в регистре USB_EPnR, 
    5)а COUNTn_TX должен быть инициализирован. 

    6)Для приема необходимо установить биты STAT_RX, чтобы разрешить прием, 
    7)и COUNTn_RX должен быть записан с выделенным размером буфера, 
    используя поля BL_SIZE и NUM_BLOCK. 
    8)Однонаправленные конечные точки, кроме массовых конечных 
    точек с изохронной и двойной буферизацией, 
    должны инициализировать только биты и регистры, связанные с 
    поддерживаемым направлением.

    9)Как только передача и/или прием разрешены, 
    зарегистрируйте USB_EPnR и местоположения ADDRn_TX/ADDRn_RX, 
    COUNTn_TX/COUNTn_RX (соответственно) не должны изменяться 
    прикладным программным обеспечением, поскольку аппаратное обеспечение 
    может изменять их значение на лету. 
    Когда операция передачи данных завершена, уведомленная 
    событием прерывания CTR, они могут быть снова доступны для 
    повторного включения новой операции.

//!###  IN packets (data transmission) ###

    При получении пакета токена IN, если полученный адрес 
    совпадает с настроенным и действительным значением конечной точки, 
    периферийное устройство USB получает доступ к содержимому 
    расположений ADDRn_TX и COUNTn_TX в записи таблицы дескрипторов 
    буфера, связанной с адресуемой конечной точкой. 
    Содержимое этих местоположений хранится во внутренних 
    16-битных регистрах ADDR и COUNT (недоступно для программного 
    обеспечения). Доступ к памяти пакетов осуществляется снова, 
    чтобы прочитать первое слово, которое должно быть передано 
    (см. Структура и использование буферов пакетов), и начинает 
    отправку PID DATA0 или DATA1 в соответствии с битом 
    USB_EPnR DTOG_TX. Когда PID завершен, первый байт слова, 
    считанный из буферной памяти, загружается в выходной сдвиговый регистр 
    для передачи на шину USB. После передачи последнего байта данных 
    вычисленный CRC отправляется. Если адресуемая конечная точка 
    недействительна, вместо пакета данных отправляется пакет 
    квитирования NAK или STALL в соответствии с битами STAT_TX 
    в регистре USB_EPnR. Внутренний регистр ADDR используется 
    в качестве указателя на текущую ячейку буферной памяти, 
    в то время как используется COUNT. подсчитать количество 
    оставшихся байтов для передачи. Каждое слово, считываемое из 
    памяти буфера пакетов, передается по шине USB, начиная с младшего байта. 
    Буферная память передачи считывается, начиная с адреса, 
    указанного ADDRn_TX для слов COUNTn_TX/2. 
    Если передаваемый пакет состоит из нечетного числа байтов, 
    будет использоваться только нижняя половина последнего доступного слова. 
    При получении хоста подтверждения ACK регистр USB_EPnR 
    обновляется следующим образом: бит DTOG_TX переключается, 
    конечная точка становится недействительной, 
    если установить STAT_TX = 10 (NAK) и установить бит CTR_TX. 
    Прикладное программное обеспечение должно сначала идентифицировать 
    конечную точку, которая запрашивает внимание микроконтроллера, 
    проверяя биты EP_ID и DIR в регистре USB_ISTR. 
    Обслуживание события CTR_TX начинает очищать бит прерывания; 
    Затем прикладное программное обеспечение подготавливает другой буфер, 
    полный данных для отправки, обновляет местоположение таблицы 
    COUNTn_TX с количеством байтов, которые должны быть переданы во 
    время следующей передачи, и, наконец, устанавливает 
    STAT_TX равным ‘11 (VALID) для повторного включения передач. 
    Хотя биты STAT_TX равны ‘10 (NAK), любой запрос IN, 
    адресованный этой конечной точке, считается NAK, что указывает 
    на условие управления потоком: хост USB будет повторять 
    транзакцию до тех пор, пока она не завершится успешно. 
    Обязательно выполнять последовательность операций в вышеупомянутом 
    порядке, чтобы не потерять уведомление о второй транзакции IN, 
    адресованной той же конечной точке, сразу после той, 
    которая вызвала прерывание CTR.

OUT and SETUP packets (data reception)
    Эти два токена обрабатываются периферийным устройством 
    USB более или менее одинаково; Различия в обработке пакетов 
    SETUP подробно описаны в следующем параграфе о передаче управления. 
    При получении PUT OUT / SETUP, если адрес соответствует 
    действительной конечной точке, периферийное устройство 
    USB получает доступ к содержимому расположений ADDRn_RX и 
    COUNTn_RX внутри записи таблицы дескрипторов буфера, связанной 
    с адресуемой конечной точкой. Содержимое ADDRn_RX хранится 
    непосредственно во внутреннем регистре ADDR. Хотя COUNT 
    теперь сбрасывается, а значения битовых полей BL_SIZE и NUM_BLOCK, 
    которые считываются в содержимом COUNTn_RX, используются 
    для инициализации BUF_COUNT, внутренний 16-битный счетчик, 
    который используется для проверки условия переполнения буфера 
    (все эти внутренние регистры недоступны программным обеспечением). 
    Байты данных, впоследствии полученные периферийным устройством USB, 
    упаковываются в слова (первый полученный байт сохраняется 
    как младший байт) и затем передаются в буфер пакетов, 
    начиная с адреса, содержащегося во внутреннем регистре ADDR, 
    в то время как значение BUF_COUNT уменьшается, а значение COUNT 
    увеличивается на передача каждого байта. 
    Когда обнаружен конец пакета DATA, проверяется правильность
    принятого CRC, и только если во время приема не было ошибок, 
    пакет квитирования ACK отправляется обратно на передающий хост. 
    В случае неправильного CRC или других видов ошибок 
    (нарушения битового содержимого, ошибки кадра и т. Д.) 
    Байты данных все еще копируются в буфер памяти пакета, 
    по крайней мере, до точки обнаружения ошибки, но пакет ACK 
    не отправляется, и Бит ERR в регистре USB_ISTR установлен. 
    Однако в этом случае обычно не требуется никаких действий 
    программного обеспечения: периферийное устройство USB 
    восстанавливается после ошибок приема и остается готовым 
    к следующей транзакции. Если адресуемая конечная точка недействительна,
    вместо ACK отправляется пакет квитирования NAK или STALL в соответствии
    с битами STAT_RX в регистре USB_EPnR, и данные не записываются 
    в буферы приемной памяти. Расположение буфера в приемной памяти 
    записывается, начиная с адреса содержится в ADDRn_RX для 
    количества байтов, соответствующих длине принятого пакета данных, 
    включая CRC (то есть длину полезной нагрузки данных + 2) 
    или до последнего выделенного местоположения памяти, 
    как определено BL_SIZE и NUM_BLOCK, в зависимости от того, 
    что наступит раньше. 
    Таким образом, периферийное устройство USB никогда не записывает 
    за пределы выделенной области буфера приемной памяти. 
    Если длина полезной нагрузки пакета данных (фактическое число байтов, 
    используемых приложением) превышает выделенный буфер, 
    периферийное устройство USB обнаруживает состояние переполнения буфера. 
    в этом случае вместо обычного ACK отправляется квитирование STALL, 
    чтобы уведомить хост о проблеме, прерывание не генерируется и 
    транзакция считается неудачной. 
    Когда транзакция завершена правильно, отправив пакет квитирования ACK, 
    внутренний COUNT регистр копируется обратно в местоположение COUNTn_RX
    внутри записи таблицы описания буфера, оставляя незатронутыми 
    поля BL_SIZE и NUM_BLOCK, которые обычно не требуют перезаписи,
    и регистр USB_EPnR обновляется следующим образом: 
    бит DTOG_RX переключается, 
    Конечная точка становится недействительной, 
    если установить STAT_RX = '10 (NAK) и установить бит CTR_RX. 
    Если транзакция не удалась из-за ошибок или из-за переполнения буфера, 
    ни одно из ранее перечисленных действий не выполняется.
    Прикладное программное обеспечение должно сначала 
    идентифицировать конечную точку, которая запрашивает 
    внимание микроконтроллера, проверяя биты EP_ID и DIR 
    в регистре USB_ISTR. Событие CTR_RX обслуживается 
    посредством первого определения типа транзакции (бит SETUP в 
    регистре USB_EPnR); прикладное программное обеспечение должно 
    очистить бит флага прерывания и получить количество принятых байтов, 
    считывающих местоположение COUNTn_RX внутри записи таблицы описания 
    буфера, связанной с обрабатываемой конечной точкой. 
    После обработки полученных данных прикладное программное обеспечение
    должно установить биты STAT_RX на ‘11 (Действительный) в USB_EPnR, 
    что позволяет проводить дальнейшие транзакции. 
    Хотя биты STAT_RX равны ‘10 (NAK), любой запрос OUT, 
    адресованный этой конечной точке, считается NAK, 
    что указывает на условие управления потоком: 
    хост USB будет повторять транзакцию до тех пор, 
    пока она не завершится успешно. 
    Обязательно выполнять последовательность операций в 
    вышеупомянутом порядке, чтобы не потерять уведомление о 
    второй транзакции OUT, адресованной той же конечной точке, 
    следующей сразу за той, которая вызвала прерывание CTR.

//!### Control transfers (Управляющие транзакции) ###
    Передачи управления выполняются из транзакции SETUP, 
    за которой следуют ноль или более этапов данных в одном 
    и том же направлении, за которым следует этап состояния 
    (передача нулевого байта в противоположном направлении). 
    Транзакции SETUP обрабатываются только конечными точками
    управления и очень похожи на OUT (прием данных), 
    за исключением того, что значения битов DTOG_TX и DTOG_RX 
    адресованных регистров конечной точки установлены в 1 и 0 
    соответственно, чтобы инициализировать передачу управления,
    и обе STAT_TX и STAT_RX установлены на '10 (NAK), чтобы 
    позволить программному обеспечению решать, должны ли 
    последующие транзакции быть IN или OUT в зависимости от 
    содержимого SETUP. Конечная точка управления должна проверять 
    бит SETUP в регистре USB_EPnR в каждом событии CTR_RX, 
    чтобы отличать обычные транзакции OUT от транзакций SETUP. 
    Устройство USB может определять количество и направление 
    этапов данных путем интерпретации данных, переданных на 
    этапе SETUP, и требуется для ОСТАНОВКИ транзакции в случае ошибок. 
    Для этого на всех этапах данных, предшествующих последнему, 
    неиспользуемое направление должно быть установлено на STALL, 
    чтобы, если хост слишком быстро меняет направление передачи, 
    он получал STALL в качестве этапа состояния. При включении 
    последнего этапа данных противоположное направление должно 
    быть установлено на NAK, так что, если хост немедленно 
    меняет направление передачи (для выполнения этапа состояния), 
    он продолжает ожидать завершения операции управления. 
    Если операция управления завершается успешно, 
    программное обеспечение изменит NAK на VALID, 
    в противном случае на STALL. В то же время, если ступенью 
    состояния будет OUT, бит STATUS_OUT (EP_KIND в регистре USB_EPnR) 
    должен быть установлен, чтобы генерировалась ошибка, 
    если транзакция состояния выполняется с ненулевыми данными. 
    Когда транзакция состояния обслуживается, приложение очищает 
    бит STATUS_OUT и устанавливает STAT_RX в значение VALID 
    (для принятия новой команды) и STAT_TX в значение NAK 
    (для задержки возможного этапа состояния сразу после следующей настройки). 
    Поскольку спецификация USB гласит, что На пакет SETUP нельзя 
    ответить с помощью рукопожатия, отличного от ACK, в конечном итоге 
    прерывая ранее введенную команду для запуска нового, логика 
    USB не позволяет контрольной точке отвечать с помощью пакета NAK 
    или STALL на токен SETUP, полученный от хоста. , Когда биты 
    STAT_RX установлены на ‘01 (STALL) или ‘10 (NAK) и получен токен 
    SETUP, USB принимает данные, выполняет необходимые передачи данных 
    и отправляет ответное подтверждение ACK. 
    Если у этой конечной точки есть ранее выданный запрос 
    CTR_RX, еще не подтвержденный приложением (т. Е. Бит CTR_RX 
    все еще установлен из ранее завершенного приема), USB отбрасывает 
    транзакцию SETUP и не отвечает ни одним пакетом квитирования 
    независимо от его состояния, моделируя ошибка приема и вынуждает 
    хост снова отправить токен SETUP. Это сделано, чтобы избежать потери
    уведомления о транзакции SETUP, адресованной той же конечной точке, 
    сразу после транзакции, которая вызвала прерывание CTR_RX
    */