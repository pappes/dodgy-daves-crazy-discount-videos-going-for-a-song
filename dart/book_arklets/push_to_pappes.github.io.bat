xcopy /y /s build ..\..\..\pappes.github.io\book_arklets
cd ..\..\..\pappes.github.io\book_arklets\build\packages
call "duplicate libraries.bat"
pause