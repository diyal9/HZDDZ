#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	/* Add By Diyal.yin 添加Win下控制台输出消息 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

    // create the application instance
    AppDelegate app;
	int nRet = Application::getInstance()->run();
    //return Application::getInstance()->run();

	/* Add By Diyal.yin 添加Win下控制台输出消息 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	FreeConsole();
#endif

	return nRet;
}
