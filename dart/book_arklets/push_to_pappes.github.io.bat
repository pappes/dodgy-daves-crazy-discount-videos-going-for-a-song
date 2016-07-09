xcopy /f /y /s /c build ..\..\..\pappes.github.io\book_arklets\build
xcopy /f /y /s /c /d /e /v /h web ..\..\..\pappes.github.io\book_arklets\build\web
xcopy /f /y /s /c web\packages ..\..\..\pappes.github.io\book_arklets\build\web\packages
copy /y web\packages\browser ..\..\..\pappes.github.io\book_arklets\build\web\packages\browser
copy /y web\packages\logging ..\..\..\pappes.github.io\book_arklets\build\web\packages\logging
copy /y web\packages\pappes_utility ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_utility
copy /y web\packages\pappes_utility\src ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_utility\src
copy /y web\packages\pappes_web_utility ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_web_utility
copy /y web\packages\pappes_web_utility\src ..\..\..\pappes.github.io\book_arklets\build\web\packages\pappes_web_utility\src
cd ..\..\..\pappes.github.io\book_arklets\build\packages
call "duplicate libraries.bat"