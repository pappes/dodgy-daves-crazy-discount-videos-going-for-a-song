call dart run build_runner build --release

mkdir ..\..\..\pappes.github.io\book_arklets
mkdir ..\..\..\pappes.github.io\book_arklets\build
mkdir ..\..\..\pappes.github.io\styles

rem xcopy /f /y /s /c means show files, dont ask for confirmation, copy subdirectories, continue on error
xcopy /f /y /s /c .dart_tool\build\generated\book_arklets ..\..\..\pappes.github.io\book_arklets\build
rem xcopy /d /e /v /h means only overwrite files if newer, include all subdirectories, verify copy, include hidden files
rem xcopy /f /y /s /c /d /e /v /h .dart_tool\build\generated\book_arklets\web ..\..\..\pappes.github.io\book_arklets\build\web
rem xcopy /f /y /s /c web\packages ..\..\..\pappes.github.io\book_arklets\build\web\packages

copy /y web\index.html ..\..\..\pappes.github.io\index.html
copy /y web\styles\main.css ..\..\..\pappes.github.io\styles\main.css
rem copy /y web ..\..\..\pappes.github.io\book_arklets\build\web
rem copy /y web\packages ..\..\..\pappes.github.io\book_arklets\build\web\packages
rem copy /y web\packages\browser ..\..\..\pappes.github.io\book_arklets\build\web\packages\browser
rem copy /y web\packages\logging ..\..\..\pappes.github.io\book_arklets\build\web\packages\logging
rem copy /y web\packages\pappes_utility ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_utility
rem copy /y web\packages\pappes_utility\src ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_utility\src
rem copy /y web\packages\pappes_web_utility ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_web_utility
rem copy /y web\packages\pappes_web_utility\src ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_web_utility\src
rem cd ..\..\..\pappes.github.io\book_arklets\build\packages
rem call "duplicate libraries.bat"