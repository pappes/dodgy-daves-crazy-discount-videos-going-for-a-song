xcopy /y /s build ..\..\..\pappes.github.io\book_arklets\build
xcopy /y /s web ..\..\..\pappes.github.io\book_arklets\build\web
cd ..\..\..\pappes.github.io\book_arklets\build\packages
call "duplicate libraries.bat"