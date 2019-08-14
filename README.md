# lacewing-0.5.4
编译好可以直接用的lacewing，在原版的基础上增加了编译成完整静态库的属性，不再依赖dll

使用时，头部增加以下内容
```
#include <windows.h>，
#include <lacewing.h>
#include <string>
#include <shlwapi.h>
#include <string>
#include <fstream> 
#include <iosfwd>

using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"shlwapi.lib")
```

同时需要在预处理器中增加宏定义`_lacewing_static`
