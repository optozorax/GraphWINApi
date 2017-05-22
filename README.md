# GraphWinApi
Класс-оболочка над функциями `WinApi` для создания окон и рисования графики в этих окнах.

# Классы
- `window`: один объект создает окно. Окно может быть разных типов, его можно видоизменять.
- `bufer`: один объект создает изображение в памяти. В него можно рисовать. Содержит графичиские примитивы со сглаживанием и без. Может существовать как в окне, так и независимо от него.
- `menu`: используется только для создания, модифицирования и хранения меню в окне.
- `WinEvents`: используется только для хранения указателей на функции-события окна и их вызова.
- `point`, `color` - для представления точки и цвета внутри программы соответственно. 

# Особенности
- Окно создается простым созданием объекта.
- Возможность делать окно на fullscreen.
- Возможность задавать цвет и прогресс в иконке программы на панели задач.
- Буфер хранит в себе цвета с альфа-каналом.
- Присутствуют примитивы для рисования как с альфа-каналом и сглаживанием(функции с использованием `AGG`), так и без этого(стандартные функции Windows).
- Возможность назначать функции на события окна.
- Возможность создавать и модифицировать меню у окна. Так же там можно даже задавать иконки для пунктов меню.
- Можно настраивать все параметры рисования текста(наклон символов, шрифт, толщину, наклон, размер и т.д.).
- Имеется функция для создания градиента из заданных цветов.
- Имеются функции для сохранения и считывания буфера в\из файла формата bmp.
- Возможность непосредственного доступа к массиву буфера, например для низкоуровнего рисования, или использования в `AGG`.
- Имеется папка tests, где находятся исходные тексты программ-примеров, использующих все эти особенности.

# Документация
Документация отсутствует. Поможет только анализ по текстам примеров, комментарии в программе и сам код.

# Список использованных библиотек:
- `AGG`(Anti-Grain Geometry). Автор: Maxim Shemanarev. Сайт: www.antigrain.com .
- `EasyBMP`. Автор: Paul Macklin. Сайт: easybmp.sourceforge.net .
 
Выражаю огромную благодарность авторам этих библиотек.

# Компиляция
- Для компиляции необходимо компилировать все cpp файлы данной библиотеки и cpp файлы используемых библиотек.
- В файле bufer2.cpp указать путь к .h файлам библиотеки `AGG`.
- В файле bufer.cpp указать путь к .h файлам библиотеки `EasyBMP`.

Программа отлично компилируется под Visual Studio 2017. 

Компиляция под mingw невозможна, из-за библиотеки shobjidl.h. Если все-же хотите скомпилировать, то убирайте все, что вызывает ошибку.

# Авторство и лицензия.
*Лицензия:* GPL.
 
*Автор:* Шепрут Илья.

*Дата:* 23.05.2017.

# Дальнейшее развитие проекта
Разработка и дальнейшее развитие прекращено, т.к. придуманная архитектура данной библиотеки была неудачной. И разработка подобной библиотеки будет произведена автором с новой архитектурой. 