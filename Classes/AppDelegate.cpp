#include "AppDelegate.h"
#include "WWMacros.h"
#include "View/LoginScene/LoginScene.h"
#include "Control/WWWorkManager/WWNetWorkManager.h"
#include "Model/WWFilter/MsgGlobalFilter.h"
#include "BaseCore\WWBI18N\WWConfigManager.h"
#include "BaseCore\WWBGUI\WWToast.h"
#include "AutoTest\TestScene.h"
#include "WWMacros.h"
#include "BaseCore/WWBLog/LogUtil.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ThirdPlatform/PlatformUtils.h"
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

	//初始化加载配置信息，必须在导演类初始化之前调用
	Configuration::getInstance()->loadConfigFile("configs/gameconfig.plist");

	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}

	// turn on display FPS (Default is false)
	bool isShowFps = Configuration::getInstance()->getValue("cocos2d.kechu.display_fps", Value(false)).asBool();
	if (isShowFps)
	{
		director->setDisplayStats(true);
	}
	// set FPS. the default value is 1.0/60 if you don't call this
	int fpsFrame = Configuration::getInstance()->getValue("cocos2d.kechu.fps", Value(60)).asInt();
	director->setAnimationInterval(1.0 / fpsFrame);

	register_all_packages();


	/*
	#多分辨率适配 @diyal.yin
	设计尺寸为 1280 * 760
	适配策略暂定用
	*/
	//	int designResolutionWidth = 1920; //长边大小
	//	int designResolutionHeight = 1080;  //窄边大小
	//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//int deviceWidth = 320;     //TODO QVGA  1.33
	//int deviceHeight = 240;
	//int deviceWidth = 480;     //TODO HVGA 1.5
	//int deviceHeight = 320;
	int deviceWidth = 960;     //todo qhd  1.77
	int deviceHeight = 540;
	//int deviceWidth = 1280;   //TODO HD 高分辨率下限   1.77
	//int deviceHeight = 720;
	//int deviceWidth = 800;      //TODO WVGA 2
	//int deviceHeight = 480;
	glview->setFrameSize(deviceWidth, deviceHeight);
#endif

	Size frameSize = glview->getFrameSize();

	float dfWeight = designResolutionSize.width / frameSize.width; //设计分辨率宽 / 屏幕分辨率宽
	float dfHeight = designResolutionSize.height / frameSize.height;  //设计分辨率高 / 屏幕分辨率高

	if (dfWeight < dfHeight) //按需拉升最小比例来处理
	{
		//保持传入的设计分辨率宽度不变，根据屏幕分辨率修正设计分辨率的高度。
		glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
	}
	else
	{
		//保持传入的设计分辨率高度不变，根据屏幕分辨率修正设计分辨率的宽度。
		glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	}

	//std::vector<std::string> searchPaths;
	//if (frameSize.height > commonResource.size.height)
	//{ 
	//	for(int i=0; i<commonResource.directory->length(); i++)
	//		searchPaths.push_back(commonResource.directory[i]);
	//	//director->setContentScaleFactor(commonResource.size.height/designResolutionSize.height);
	//}
	//else
	//{ 
	//	for(int i=0; i<retinaResource.directory->length(); i++)
	//		searchPaths.push_back(retinaResource.directory[i]);
	//}

	//FileUtils::getInstance()->setSearchResolutionsOrder(searchPaths); //设置资源搜索路径


	log("Now FrameSize is : %d x d% | DesignSize is : %d x d%", 
		glview->getFrameSize().width, glview->getFrameSize().height, 
		designResolutionSize.width, designResolutionSize.height);

	director->setProjection(Director::Projection::_2D); 

	/* 取得设备信息 */
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PlatformUtils::getDeviceBaseInfo();
	/*int persent = PlatformUtils::getBatteryPersent();
	log("Platform batteryPersent2 %d", persent);*/
#endif

	/* 设置保持屏幕 */
	Device::setKeepScreenOn(true);

    /*
	* 账号信息初始化
	*/
	//AccountUtils::getInstance()->init();


	/*
	* 音频文件预加载 
	*/

	/*
	* 资源更新 （静默更新、或最小资源包初始下载）
	*/


	/*
	* 字体缓存 retain 
	*/

	/*
	*文字预加载
	*/
	//WWConfigManager::getInstance()->loadXML("XML/WWString.xml");
	/*
	*config.xml预加载
	*/
	//WWConfigManager::getInstance()->loadXML(CONFIG_XML_PATH.c_str());

	// create a scene. it's an autorelease object

	/* 自动化测试相关
	*  如果需要自动化测试，请在配置文件中修改
	*/
	int dm = Configuration::getInstance()->getValue("cocos2d.kechu.debugmode", Value(0)).asInt();

	if (DebugMode == 1)
	{
		auto m_TestScene = TestScene::create();
		Director::getInstance()->replaceScene(m_TestScene);
	}
	else
	{
		auto pScene = LoginScene::create();
		director->runWithScene(pScene);
	}

	/* 日志初始化设置 */
	bool openWriteLog = Configuration::getInstance()->getValue("cocos2d.kechu.writelog", Value(false)).asBool();
	if (openWriteLog)
	{
		std::string filename = Configuration::getInstance()->getValue("cocos2d.kechu.logfilename", Value("WaWaDDZLog")).asString();
		SETLOGFILENAME(filename)
		int logLevel = Configuration::getInstance()->getValue("cocos2d.kechu.loglevel", Value("WaWaDDZLog")).asInt();
		SETLOGLEVEL(logLevel)
		wwonlylog("----->Start of this app ");
		wwlog(LOG_DEBUG, "AppDelegate", "当前为Debug版本");
	}
	else
	{
		SETLOGENABLE(false);
	}


	/*
	* 内存泄漏检查
	*/
	//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//	log("@************VM Check By (if you found the memory exception please tell diyal ");
	//	log("VM MEMORY USED: %0.2f KB", Director::getInstance()->getTextureCache()->getCachedTextureInfo());
	//	Director::getInstance()->getTextureCache()->getCachedTextureInfo();
	//#endif

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
