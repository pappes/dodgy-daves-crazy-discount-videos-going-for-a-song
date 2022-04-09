echo Note: publishing to github will take 30 mins before published source is available on pappes.github.io
call dart run build_runner build --release

mkdir ..\..\..\pappes.github.io\book_arklets
mkdir ..\..\..\pappes.github.io\book_arklets\build
mkdir ..\..\..\pappes.github.io\book_arklets\build\web
mkdir ..\..\..\pappes.github.io\book_arklets\build\styles

rem xcopy /f /y /s /c means show files, dont ask for confirmation, copy subdirectories, continue on error
xcopy /f /y /s /c .dart_tool\build\generated\book_arklets ..\..\..\pappes.github.io\book_arklets\build

copy /y web\index.html ..\..\..\pappes.github.io\book_arklets\build\index.html
copy /y web\styles\main.css ..\..\..\pappes.github.io\book_arklets\build\styles\main.css