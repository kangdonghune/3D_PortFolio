// 명령어			옵션				원본 파일이 있는 경로			새로 저장할 경로

xcopy			/y				.\Engine\System\Code\*.h		.\Reference\Header
xcopy			/y				.\Engine\Header\*.*				.\Reference\Header

xcopy			/y				.\Engine\System\Bin\System.dll	.\Client\Bin
xcopy			/y				.\Engine\System\Bin\System.lib	.\Reference\Library

xcopy			/y				.\Engine\System\Bin\System.dll	.\MFC\Bin
