#
\[En\]
#
# Introduction.
TeleNEXus is a program for building SCADA (Supervisory Control And
Data Acquisition) and ICS (Industrial Control System) systems.
The system project is a set of extensible markup language (XML) files that 
describe the graphical interface, data sets and their presentation methods.
The software basis of TeleNEXus is the Qt framework, due to which
provides cross-platform.

# Features.
 - Cross-platform;
 - For the development of the project you need only a text editor;
 - Security system to restrict access to the functions described
   systems;
 - Ability to work in embedded systems with touchscreen;
 - Built-in ability to create on-screen keyboards;
 - Ability to use JavaScript to describe behavior
   systems, data processing, and response to operator actions;
 - Ability to connect plugins in the form of dynamic libraries for
   expanding the possibilities of building a graphical interface and implementing
   data transfer protocols.

# Compiling.

## Composition of build.
  The project consists of three separately compiled parts:
- The main part of TeleNEXus located in the folder `tnexcore`;
- The launcher located in the `tools/launcher` folder;
- MODBUS data source implementation plugin in the `plugins/modvussource` folder.

## Version Qt.
 TeleNEXus was build using Qt 5.12.8.

## Linux.
  To fully compile the entire project, you can use the bash script file
`build.sh` with the `-jN` option indicating the number of compilation threads, or you can
compile all parts separately using qmake and make.
After compilation, you can use the install.sh script which will copy
executable files and plugins to the `/opt/tnex` and `/opt/tnex/plugins` folders, as well as
create a tnex link in `/usr/bin/`

  Build and installation example:
  ```
  $./build.sh
  $sudo ./install.sh
  ```

## Windows.
  If you have the Linux Subsystem for Windows installed on your system, you can
use the bash script file `win_build.sh` with the `-jN` option to specify
number of compilation threads.
  The `win_build.sh` file contains substitutions for the qmake and make commands at the beginning
which need to be changed in accordance with the utilities you have installed.
  You can use QtCreator to compile all parts individually. For this
need to open files with `.pro` extension in QtCreator and after settings
projects to compile.
  After compiling the project, you need to deploy it using
Qt utility `windeployqt.ext`, or if compilation was done by a script
`win_build.sh`, you can use the `win_deploy.sh` script as a result of work
which the `win_deploy` folder should appear containing all executable files
and libraries necessary for the program to work.

  Build and deploy example:
  ```
  $./win_build.sh
  $./win_deploy.sh
  ```
  or
  ```
  $bash win_build.sh
  $bash win_deploy.sh
  ```

# Usage.
## Console.
   There are two ways to run a project in the console:
   - Go to the project folder and run the tnex command, this will produce
   search for the root file of the project with the name `main.xml`;
   - Run a command like `tnex "xml file name or path to project"`
## tnexluncher project manager.
   Using this small program, you can quickly launch different projects.
   The program has a simple graphical interface and supports all possible
   project launch options

# Examples.
  Examples of simple projects designed to familiarize with the functions 
  of the program are given in the folder './examples'

## Screenshots

  Screenshot of the sample project with the scheme "Set of light bulbs and buttons"
  `examples/example_projects/mnemoscheme`.

![Screenshot](./docs/pictures/scheme_example.png)

  Screenshot of widget style examples
  `examples/widgetstyles`.

![Screenshot](./docs/pictures/widget_styles_example.png)

  Screenshot of the on-screen keyboard example
  `examples/keyboard`.

![Screenshot](./docs/pictures/keyboard_example.png)

#
\[RU\]
#
# Введение.
TeleNEXus это программа для построения SCADA (Supervisory Control And 
Data Acquisitionи) и ICS (Industrial Control System) систем. 
Проект ситсемы представляет собой набор файлов расширяемого языка разметка 
(XML) описывающие графический интерфейс, наборы данных и спобобы их представления.
Програмной основой TeleNEXus является фреймворк Qt за счет чего 
обеспечивается кроссплатформенность.

# Особенности.
- Кроссплатформенность;
- Для рызработки проекта нужен только текствый редактор;
- Система безопасности для ограничения доступа к функциям описываемой
  системы;
- Возможность работы во встроенных системах с экранами TouchScreen;
- Встроенная возможность создания экранных клавиатур;
- Возможность использования скриптов JavaScript для описания поведения
  системы, обработки данных, и реакции на действия оператора;
- Возможность подключения плагинов в виде динамических библиотек для
  расширения возможностей построения графического интерфейса и реализации
  протоколов передачи данных.

# Компилирование.

## Состав сборки.
  Проект состоит из трех отдельно компилируемых частей:
- Основная часть TeleNEXus расположенная в папке `tnexcore`;
- Программа запуска расположенная в папке `tools/launcher`;
- Плагин реализации источника данных MODBUS в папке `plugins/modvussource`.

## Версия Qt.
  Для сборки TeleNEXus использовалась версия Qt 5.12.8.

## Linux.
  Для полной компиляции всего проекта можно исользовать файл bash скрипта
`build.sh` с параметром `-jN` указывающий количество потоков компиляции, или можно
компилировать все части по отдельности, используя утилиту qmake и make.
После компиляции можно использовать скрипт install.sh который скопирует
исполняемые файлы и плагины в папку `/opt/tnex` и `/opt/tnex/plugins`, а также
создаые ссылку tnex в папке `/usr/bin/`

  Пример сборки:
  ```
  $./build.sh
  $sudo ./install.sh
  ```

## Windows.
  Если в вашей системе установлена подсистема Linux для Windows можно 
исользовать файл bash скрипта `win_build.sh` с параметром `-jN` указывающий 
количество потоков компиляции.
  В файле `win_build.sh` в начале указаны подстановки для команды qmake и make
которые необходимо изменить в соотвенствии с установленными у вас утилитами.
  Вы можете QtCreator, чтобы компилировать все части по отдельности. Для этого 
необходимо открыть файлы с расширением `.pro` в QtCreator и после настроек 
проектов произвести компиляцию.
  После компиляции проекта необходимо произвести его развертывание используя 
утилиту Qt `windeployqt.ext`, или если компиляция производилась скриптом  
`win_build.sh`, можно использовать скрипт `win_deploy.sh` в результате работы 
которого должна появиться папка `win_deploy` содержащая все исполняемые файлы
и библиотеки небоходимые для работы программы.

  Пример сборки:
  ```
  $./win_build.sh
  $./win_deploy.sh
  ```
  or
  ```
  $bash win_build.sh
  $bash win_deploy.sh
  ```

# Использование.
## Консоль.
  Для запуска проекта в консоли можно использовать два пути:
  - Перейти в папку проекта и запустить команду tnex, при этом будет произведен
  поиск корневого файла проекта с именем `main.xml`;
  - Выполнить команду вида `tnex "xml file name or path to project"`
## Менеджер проектов tnexluncher.
  Используя эту небольшую программу можно быстро запускать разные проекты.
  Программа имеет простой графический интерфейс и поддерживает все возможные
  варианты запуска проекта.

# Examples.
  Examples of simple projects designed to familiarize with the functions 
  of the program are given in the folder './examples'

## Снимки экрана

  Снимок экрана простого проекта схемы типа "Набор лампочек и кнопочек"
  `examples/example_projects/mnemoscheme`.

![Screenshot](./docs/pictures/scheme_example.png)

  Снимок экрана проекта примеров стилей оформления виджетов
  `examples/widgetstyles`.

![Screenshot](./docs/pictures/widget_styles_example.png)

  Снимок экрана примера экранной клавиатуры
  `examples/keyboard`.

![Screenshot](./docs/pictures/keyboard_example.png)

