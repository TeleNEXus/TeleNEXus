" All system-wide defaults are set in $VIMRUNTIME/debian.vim and sourced by
" the call to :runtime you can find below.  If you wish to change any of those
" settings, you should do it in this file (/etc/vim/vimrc), since debian.vim
" will be overwritten everytime an upgrade of the vim packages is performed.
" It is recommended to make changes after sourcing debian.vim since it alters
" the value of the 'compatible' option.

" This line should not be removed as it ensures that various options are
" properly set to work with the Vim-related packages available in Debian.
runtime! debian.vim

" Vim will load $VIMRUNTIME/defaults.vim if the user does not have a vimrc.
" This happens after /etc/vim/vimrc(.local) are loaded, so it will override
" any settings in these files.
" If you don't want that to happen, uncomment the below line to prevent
" defaults.vim from being loaded.
" let g:skip_defaults_vim = 1

" Uncomment the next line to make Vim more Vi-compatible
" NOTE: debian.vim sets 'nocompatible'.  Setting 'compatible' changes numerous
" options, so any other options should be set AFTER setting 'compatible'.
"set compatible

" Vim5 and later versions support syntax highlighting. Uncommenting the next
" line enables syntax highlighting by default.
if has("syntax")
  syntax on
endif

" If using a dark background within the editing area and syntax highlighting
" turn on this option as well
"set background=dark

" Uncomment the following to have Vim jump to the last position when
" reopening a file
"if has("autocmd")
"  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
"endif

" Uncomment the following to have Vim load indentation rules and plugins
" according to the detected filetype.
"if has("autocmd")
"  filetype plugin indent on
"endif

" The following are commented out as they cause vim to behave a lot
" differently from regular Vi. They are highly recommended though.
set showcmd		" Show (partial) command in status line.
"set showmatch		" Show matching brackets.
"set ignorecase		" Do case insensitive matching
"set smartcase		" Do smart case matching
"set incsearch		" Incremental search
"set autowrite		" Automatically save before commands like :next and :make
"set hidden		" Hide buffers when they are abandoned
set mouse=a		" Enable mouse usage (all modes)

set encoding=utf-8
set nocompatible
set number
set textwidth=80
set cursorline

set bs=indent,eol,start " backspace без глюков
set nocompatible " несовместимость с VI
set wrap " включаем перенос строк
set linebreak " перенос по словам, а не по буквам
set showcmd " просмотр выполняемой команды в правом нижнем углу
set ruler " показывать строку с позицией курсора
set showmatch " показывать первую парную скобку после ввода второй
set showtabline=2 " всегда показывать строку вкладок
"set laststatus " всегда показывать строку статуса
set title " показывать имя буфера в заголовке терминала
set confirm " использовать диалоги вместо сообщения о ошибках
"set visualbell " мигание вместо звукового сигнала
set hidden " не выгружать буфер при переключении на другой файл
set t_Co=256 " использовать больше цветов в терминале
set background=dark " задаем фон терминала, влияет на подсветку синтаксиса
set smartindent " включаем умную автоматическую расстановку отступов
set autoindent " наследовать отступы предыдущей строки
set tabstop=4 " размер табуляции
set shiftwidth=4 " размер сдвига при нажатии на клавиши < или >
set cursorline " подсветка текущей строки
"set autoread " автоматическое перечитывание файла при изменении
set pastetoggle= " сохраняет отступы при вставке фрагмента
"set splitbelow " разбивать окно горизонтально снизу
"set splitright " разбивать окно вертикально справа
set foldmethod=syntax " фолдинг (сворачивание блоков кода) (zc - свернуть, zo - развернуть)
"set completeopt=longest,menuone "автодополнение 
set wildmenu " Включение дополнительного меню выбора автодоколнения


" Source a global configuration file if available
if filereadable("/etc/vim/vimrc.local")
  source /etc/vim/vimrc.local
endif




" Небольшое объяснение по синтаксису мапинга
" map последовательность_клавиш действие
"
" Клавиши:
"
" Shift: <S-...>
" Ctrl: <C-...>
" Meta: <M-...>
" Alt: <A-...>
"
" Пример: map <S-s>k dd //в этом примере последовательное нажатие Sk удалит строку
"
" <F1>...<F12>
"
" Enter: <CR> или <Enter> или <Return>
" Пробел: <Space>
" Esc: <Esc>
"
" РЕЖИМЫ
"
" Normal: режим ввода команд
"
" Visual: режим “визуального” выделения блоков текста, к которым затем применяются команды Normal режима
"
" Operator-pending: режим, в котором Vim ожидает дополнительного “поясняющего” ввода (например после " нажатия клавиш “d”, “y”, “c”, и т.п.).
"
" Insert: режим непосредственной вставки текста
"
" Command-line: режим, в котром находится редактор после нажатия клавиш “:” или “/”
"
" Lang-Arg: режим поиска
"
" ОБЛАСТЬ ДЕЙСТВИЯ ЗАБИНДЕННЫХ КЛАВИШ
"
"map
" Normal, Visual, Operator-pending
"map!
" Insert, Command-line
"nmap
" Normal
"vmap
" Visual
"omap
" Operator-pending
"cmap
" Command-line
"imap
" Insert
"lmap
" Insert, Command-line, Lang-Arg

map <C-c> y<CR> "позволяет копировать текст нажатием Ctrl+c
map <C-v> p "позволяет вставлять текст нажатием Ctrl+v
imap <C-v> <ESC>pi "тоже вставка текста, только в режиме редактирования
vmap <C-c> y<CR> "копирование текста, только в визуальном режиме

"следующие четыре мапинга позволяют переключаться между открытыми в VIM'e окнами нажатием alt+стрелки
map <A-Left> <C-w><Left> "
map <A-Right> <C-w><Right> "
map <A-Up> <C-w><Up>
map <A-Down> <C-w><Down>

"это биндинги для фолдинга. позволяют свертывать/разворачивать код в обычном режиме нажатием -/+
map + zo
map - zc

"эти биндинги для меню. благодоря этим биндингам можно нажатиями F1, F2, F3, F4 вызывать различные меню
set wildmenu
set wcm=<Tab>
menu Taglist.Open/Close :TlistToggle<CR> "открытие taglist
menu Taglist.Reload :w<CR><C-w><Left>u <C-w><Right> "обновление taglist
menu Folding.Open zo "открыть свернутый код
menu Folding.Close zc "скрыть развернутый код
menu Folding.Open_all zR "открыть все свертки
menu Folding.Close_all zM "закрыть все свертки
menu File.Save :w<CR> "сохранение файла
menu File.Load_new_window :sp +Explore<CR> "запуск встроеного файлового менеджера в новом окне
menu File.Load_and_save :w<CR> :Explore<CR> сохранение и запуск файлового менеджера
menu File.Load :Explore<CR> "запуск файлового менеджера в новом окне
menu File.Exit_and_Save :wq<CR>:q!<CR>:q!<CR>:q!<CR>:q !<CR>:q!<CR>:q!<CR> "полный выход и сохранение
menu File.Exit :q!<CR>:q!<CR>:q!<CR>:q!<CR>:q !<CR>:q!<CR>:q!<CR> "полный выход без сохранения
menu Project.Compile :w<CR>:!clear;g++ %<CR> "компиляция проекта (С++)
menu Project.Run :w<CR>:!clear;./a.out<CR> "запуск проекта
menu Project.Debug :w<CR>:!clear;gdb a.out<CR> "отладка проекта с помощью GDB

"мапинги длля вызовов меню
map <F7> :emenu File.<Tab>
map <F2> :emenu Project.<Tab>
map <F3> :emenu Taglist.<Tab>
map <F4> :emenu Folding.<Tab>
map <C-Tab> <ESC> tabNext<CR>
imap <F1> <ESC>:emenu File.<Tab>
imap <F4> <ESC>:emenu Folding.<Tab>
imap <F2> <ESC>:emenu Project.<Tab>
imap <F3> <ESC>:emenu Taglist.<Tab>
vmap <F1> <ESC>:emenu File.<Tab>
vmap <F4> <ESC>:emenu Folding.<Tab>
vmap <F2> <ESC>:emenu Project.<Tab>
vmap <F3> <ESC>:emenu Taglist.<Tab> 

