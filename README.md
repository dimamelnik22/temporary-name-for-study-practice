## от Артура:
## Зачем нужно переделывать архитектуру:
  Проблемы текущей архитектуры : неудобная система из step, substep в которой для каждого npc проходят substep, но step
меняется только после полного обхода всех npc, игрока и GOL. Обрывки функциональности торчат в классах, в которых их быть не 
должно. Многие действия сделаны внутри функций, но как я узнал, все функции выполняются в течении 1 кадра, поэтому там 
например нельзя ставить внутри delay, и могут возникнуть проблемы с анимацией. То есть действия надо перенести с функций на 
эвенты. Также использование interface в BP сильно облегчает жизнь, чем я воспользуюсь в новой архитектуре.

  Идея новой архитектуры - есть Turns и Rounds. Создаем массив всех сущностей, в порядке - все игроки-люди, затем все NPC, 
затем GOL и прочие подобные ей. Благодаря interface с ними всеми можно взаимодйствовать единым способом. А именно - 
передавать им message StartTurn. Сущность его получает и ходит по своим правилам. Когда она истратила свои очки (creature) 
или обновила поле (GOL), она сама вызывает свой эвент EndTurn, который заканчивает ход и вызывет NextTurn уже в TurnBased. Когда пройдем по всему массиву, то удалим всн Null из массива и вызовем NextRound.


----------------------------------------------------------------------------------------------
# Life Of Game

## Основа - процедурно-генерируемые уровни

### Сделано:

+ Генерация общего ландшафта в виде не связных областей

+ Добавление коридоров между областями для получения целостной области

+ Определение старта/финиша

  
## GoL - Динамические препятствия:

### Сделано:

+ Полный функционал модели GoL с правилами, задаваемыми как параметры

+ Хранение и вставка паттернов

### Планируется:

- Классификация и структурированное хранение паттернов, их анализ и комбинация

- Автоматический поиск паттернов и практичных начальных конфигураций GoL для полученных на первом этапе генерации ландшафтов

- Автоматический анализ разрешимости уровня

## Действия игрового персонажа на уровне:

Персонаж начинает игру в случайно выбранной точке уровня. Целью в прототипе является поиск случайно генерируемого выхода. С добавлением сюжета цели могут быть изменены на «Победить босса», «Найти предмет», «Найти указанного персонажа и поговорить с ним» и т.д.

Игроку противостоят враги различного типа и сам игровой мир. Угроза окружения предстает в виде «проклятой земли» - клеток, нахождение в которых наносит урон персонажу. Перемещение «проклятой земли» происходит по правилам, основанным на GoL.

Сама игра GoL имеет конфигурацию B3/S23. Мы же хотим сделать так, чтобы раз в какое-то фиксированное число ходов эта конфигурация случайно менялась на какую-либо другую. Например, на B34/S23456. Игрок будет видеть, какая конфигурация будет следующей, чтобы знать, чего ожидать.

Эта система добавляет стратегический аспект в геймплей, так как будет необходимо продумывать не только поединки с врагами, но еще и обычное перемещение.

Игрок и все прочие персонажи ходят по очереди, порядок хода определяется системой. Игрок в основном ходит первым. Последним идёт ход GoL - «проклятой земли».

На один ход игроку или NPC даётся несколько очков действий, которые тратятся на передвижение и на атаку. Потенциально, некоторые действия, как, например, передвижение по пересечённой местности, будет отнимать больше очков действий, чем обычно.

У игрока есть инвентарь с предметами. Предметы можно находить в мире или подбирать с убитых врагов.

### Сделано:

+ Система пошаговой игры

+ Передвижение по полю

+ Подбираемые предметы

### Планируется:

- Расстановка предметов и тайлов уровня на карте

- «Проклятая земля»

- Инвентарь

- AI NPC

- Различные враги

- Система навыков персонажа с использованием GoL

## Сражения:

Как только игрок решает напасть на NPC (или NPC напасть на игрока, или NPC напасть на другого NPC), у участников сражений проставляется флажок “В бою”.

Во время боя персонажи по тем же правилам могут двигаться, подбирать предметы, открывать сундуки и прочие нейтральные действия, но не могут взаимодействовать с мирными персонажами. Помимо этих стандартных действий у персонажа также будет возможность атаковать противника. Атака выглядит след. образом: персонаж выбирает одно действие из возможных (об этом ниже), выбирает цель для атаки (ибо, потенциально, сражения могут происходить против группы противников) и атакует ее. Действий также, как и вне боя, ограниченное количество. Более того, «боевые» действия потенциально могут иметь разную стоимость (одно действие стоить два очка действий).
Успешность атаки зависит от характеристик персонажей и воли случая. Как пример можно привести работу «стандартной атаки». У нас есть параметр «защиты» у обороняющегося персонажа (ОП) и параметр «атаки» у нападающего (НП). Когда НП решил использовать «стандартную атаку» против ОП, игра определит успешность нападения следующим образом: если параметр «Атаки» у НП + случайно выбранное число от 1 до 20 будет больше, чем параметр «Защиты» у ОП, то атака проходит; в ином случае атака считается безуспешной.

Вернемся к «выбору действий из возможных». У каждого персонажа есть своя «боевая табличка» (или «боевая матрица», чтобы звучало покруче). Это просто таблица размера, условно, 5 на 5 (потенциально, разные ТИПЫ персонажей (разные типы врагов; игрок) обладают разными таблицами, разной размерности и с разным содержимым). На ней находится какой-то циклический паттерн из GoL (например, семафор), который одинаков для всех персонажей одного типа. Соответственно, ячейки таблицы могут быть либо «живыми», либо «мёртвыми». Перед началом боя игра случайным образом заполняет все ячейки, которые потенциально могут быть «живыми», действиями, которые может использовать персонаж на протяжении данного боя. Ячейки таблицы меняют свою «живучесть» по алгоритмам GoL в конце хода персонажа. Персонаж может совершать только те действия, которые на момент начала его хода находятся на «живых» ячейках таблицы. Действие «стандартная атака» должно быть доступно всегда. Это должно поддерживаться за счёт выбора соответствующих паттернов. (Напомню, что помимо «боевых» действия, персонаж может также двигаться, подбирать предметы и прочее). 

##### Действия для «боевой матрицы» берутся отсюда:

- «Оборона» (укрепляет параметр «Защиты» пропорционально оставшемуся числу действий и заканчивает ход)

- Набор активируемых способностей у данного персонажа (отличается у разных типов NPC и игрока, но одинаков у разных персонажей одного типа). Например, «Точный удар» (только для ближнего боя; стоит на одно действие больше, но при использовании увеличивает атаку на 5 и уменьшает защиту на 3 на 1 ход), «Мощный выстрел» (только для дальнего боя; при использовании увеличивает урон на 5 и уменьшает атаку на 2 на 1 ход).

- Набор используемых предметов у данного персонажа (потенциально может быть различен у всех). Например, «Зелье лечения», «Отравленный дротик» и т.п.

- Набор заклинаний у данного персонажа (аналогично способностям, все в пределах одного типа имеют один и тот же набор; разные типы имеют потенциально разные наборы). Например, «Лечение», «Пламя» и т.п.

Сражение заканчивается, когда рядом с победителями не останется живых противников (т.е. персонажей с поставленным флажком «В бою»).

### Сделано:

+ Написан концепт

### Планируется:

- Доработка и реализация концепта



