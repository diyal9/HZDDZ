package com.wawa.hzddz.utils;

import java.lang.reflect.Method;
import java.util.Locale;
import java.util.Random;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.telephony.PhoneStateListener;
import android.telephony.SignalStrength;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;


/**
 * 跨平台混编 工具类
 * 
 * @author dial.yin
 * 
 */
public class PlatformUtils {
	
	
	/** wifi */
	public static final String WIFI = "wifi";
	/** 3G wap 中国移动3g wap APN */
	public static final String CMWAP = "cmwap";
	/** 3G net 中国移动3g net APN */
	public static final String CMNET = "cmnet";
	/** wap 中国移动 wap APN */
	public static final String GWAP_3 = "3gwap";
	/** 3G wap 中国联通3gwap APN */
	public static final String GNET_3 = "3gnet";
	/** 3G net 中国联通3gnet APN */
	public static final String UNIWAP = "uniwap";
	/** uni wap 中国联通uni wap APN */
	public static final String UNINET = "uninet";
	/** uni net 中国联通uni net APN */
	public static final String CTNET = "ctnet";
	/** uni net 中国电信ct net APN */
	public static final String CTWAP = "ctwap";

	private static final Uri PREFERRED_APN_URI = Uri
			.parse("content://telephony/carriers/preferapn");
	
	private static int m_PhoneNetLevel = 0;
	
	
	// TODO 更换项目后，更换返回值调用的方法
	public static Activity getActivity() {
		Activity activity = null;
		try {
			Class clazz = Class.forName("org.cocos2dx.cpp.AppActivity");
			Method method = clazz.getMethod("getContext", new Class[] {});
			activity = (Activity) method.invoke(clazz, new Object[] {});
		} catch (Exception e) {
			e.printStackTrace();
		}
		return activity;
	}

	public static Context getContext() {
		Context context = null;
		try {
			Class clazz = Class.forName("org.cocos2dx.cpp.AppActivity");
			Method method = clazz.getMethod("getContext", new Class[] {});
			context = (Context) method.invoke(clazz, new Object[] {});
		} catch (Exception e) {
			e.printStackTrace();
		}
		return context;
	}
	
	/**
	 * 获取基本
	 * 
	 * @return
	 */
	public static void getDeviceBaseInfo() {
		boolean isConnNet = isConnect();//判断是否联网
		System.out.println("Platform 是否联网:" + isConnNet);
		
		String strMac = getMacAddress();//获取mac地址
		System.out.println("Platform Mac:" + strMac);
		
		String strIMEI = getIMEI();//IMEI
		System.out.println("Platform IMEI:" + strIMEI);
		
		String strIMSI = getIMSI();//IMSI
		System.out.println("Platform IMSI:" + strIMSI);
		
		int width = getScreenWidth();//屏幕宽
		System.out.println("Platform width:" + width);
		
		int height = getScreenHeight();//屏幕高
		System.out.println("Platform height:" + height);
		
		int IdCode = getNetworkOperatorCode();//运营商id
		System.out.println("Platform idcode:" + IdCode);
		
		String mobileType = getPhoneMANUFACTURER();//手机型号
		System.out.println("Platform MobileType:" + mobileType);
		
		String phoneModel = getPhoneModel();//手机厂商
		System.out.println("Platform phoneModel:" + phoneModel);
		
		String apnType = checkNetState();//网络接入方式
		System.out.println("Platform apnType:" + apnType);
		
		String SDkVersion = getSDkVersion();//OS
		System.out.println("Platform SDkVersion:" + SDkVersion);
		
		String ipAddress = getIpAddress();
		System.out.println("Platform ipAddress:" + ipAddress);
		
		String phoneNum = getNativePhoneNumber();
		System.out.println("Platform phoneNum:" + phoneNum);
		
		getDeviceBaseInfoCallBack(isConnNet, strMac, strIMEI, strIMSI,
				width,height,IdCode,mobileType,phoneModel,apnType,SDkVersion,ipAddress,phoneNum);
	}
	
	/**
	 * 设备信息返回 调用C++，返回设备信息
	 * 
	 * @return isConnNet 是否联网
	 */
	public static native void getDeviceBaseInfoCallBack(
			Boolean isConnNet,    //是否连接网络
			String strMac,        //mac地址
			String strIMEI,       //IMEI
			String strIMSI,       //IMSI
			int width,            //屏幕宽
			int height,           //屏幕高
			int IdCode,           //运营商id
			String mobileType,    //手机型号
			String phoneModel,    //手机机型
			String apnType,       //联网类型
			String SDkVersion,    //os
			String ipAddress,     //ip地址
			String phoneNum       //电话号码
			);
	
	/**
	 * 获取电量 提供C++调用
	 */
	public static int getBatteryPersent(){
		int intBattery = getBatteryStatus();
		  System.out.println("电量：" + intBattery);
		return intBattery;
	}
	
	/**
	 * 获取信号强度 提供C++
	 * @return
	 */
	public static int getNetStatus(){
		int ret = 0;
		if (checkNetState().equalsIgnoreCase("wifi")) {
			ret = getWifistatus();
		}else{
			ret = getPhoneNetState();
		}
		return ret;
	}
	
	/**
	 *  获取WiFi信号强度
	 * @return
	 */
	public static int getWifistatus() {
		WifiManager wifiManager = (WifiManager) getActivity()
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();
		if(wifiManager.getWifiState()<3){
			return 0;
		}
		int level = wifiInfo.getRssi();
		if (level <= 0 && level >= -50) {
			return 4;
		} else if (level < -50 && level >= -70) {
			return 3;
		} else if (level < -70 && level >= -80) {
			return 2;
		} else if (level < -80 && level >= -100) {
			return 1;
		} else {
			return 0;
		}
	}
		
	public class MyPhoneStateListener extends PhoneStateListener{
		@Override
		public void onSignalStrengthsChanged(SignalStrength signalStrength) {
			super.onSignalStrengthsChanged(signalStrength);
			if(!checkNetState().equals(WIFI)){
				int gsmSignalLevel = Math.abs(signalStrength.getGsmSignalStrength());
				if (gsmSignalLevel <= 25) {// 信号最好
					m_PhoneNetLevel = 3;
				} else if (gsmSignalLevel <= 60) {// 信号较好
					m_PhoneNetLevel = 2;
				} else if (gsmSignalLevel <= 70) {// 信号偏差
					m_PhoneNetLevel = 1;
				} else if (gsmSignalLevel <= 100) {// 最差，有可能连接不上或者掉线
					m_PhoneNetLevel = 0;
				}
			}
		}
	}
	
	/**
	 * 获取手机信号强度
	 * @return
	 */
	public static int getPhoneNetState(){
		return m_PhoneNetLevel;
	}
	
	public static String getNativePhoneNumber(){
		String phoneNum = "";
		TelephonyManager telephonyManager = (TelephonyManager) getActivity()
                .getSystemService(Context.TELEPHONY_SERVICE);
		phoneNum = telephonyManager.getLine1Number();
		System.out.println("Platform phoneNum1 =" + phoneNum);
		return phoneNum;
	}
	
    //获取屏幕的宽度 
	public static int getScreenWidth() { 
		DisplayMetrics  dm = new DisplayMetrics();     
		   
		getActivity().getWindowManager().getDefaultDisplay().getMetrics(dm);     
		   
		int screenWidth = dm.widthPixels; 
		return screenWidth; 
	} 
	
	
	//获取屏幕的高度 
	public static int getScreenHeight() { 
		DisplayMetrics  dm = new DisplayMetrics();     
		   
		getActivity().getWindowManager().getDefaultDisplay().getMetrics(dm);  
		int screenHeight = dm.heightPixels;
		return screenHeight; 
	}
	
	
	
	/**
	 * 获取用户识别码，前5位是运营商id
	 */
	public static String getIdCode() {
		String idCode = "";
		try {
			TelephonyManager tm = (TelephonyManager) getActivity()
					.getSystemService(Context.TELEPHONY_SERVICE);

			if (tm.getSubscriberId() != null
					&& tm.getSubscriberId().length() != 0) {
				idCode = tm.getSubscriberId();
			}
		} catch (Exception e) {
		}

		return idCode;
		// TODO for test
		// return "460230912121001";

	}

	/**
	 * 获取手机android版本号
	 */
	public static String getSDkVersion() {
		String version = "";
		if (!Build.VERSION.RELEASE.equals("")) {
			version = "ANDROID" + Build.VERSION.RELEASE;
		}
		return version;
	}

	/**
	 * 获取手机厂商
	 */
	public static String getPhoneModel() {
		return Build.MANUFACTURER == null ? "" : Build.MANUFACTURER;
	}

	/**
	 * 手机型号
	 */
	public static String getPhoneMANUFACTURER() {

		return Build.MODEL == null ? "" : Build.MODEL;
	}

	
	/**
	 * 获取设备唯一标示
	 * 
	 * @return
	 */
	public static String getPhoneDeviceId() {
		Activity activity = getActivity();

		// 如果随机码存在时 优先使用随机码登录
		String deviceId = null;
		String time = null;
		StringBuffer sb = new StringBuffer();
		try {
			TelephonyManager tm = (TelephonyManager) activity
					.getSystemService(Context.TELEPHONY_SERVICE);
			String imei = tm.getDeviceId();
			if (imei != null && imei.length() != 0) {
				return imei;
			}
			String imsi = tm.getSubscriberId();
			if (imsi != null && imsi.length() != 0) {
				return imsi;
			}

			time = "" + System.currentTimeMillis();

			String allChar = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

			if (time.length() < 16) {
				Random random = new Random();
				for (int i = 0; i < 16; i++) {
					sb.append(allChar.charAt(random.nextInt(allChar.length())));
				}
			}
		} catch (Exception e) {
			Log.i("Platfrom","###############getPhoneDeviceId catch exception ############");
		}

		deviceId = time + sb.toString();

		return deviceId == null ? "" : deviceId;
	}
	
	/**
	 * 获取版本号
	 * 
	 * @return
	 */
	public static int getVersionCode() {
		try {
			return getActivity().getPackageManager().getPackageInfo(
					getActivity().getPackageName(), 0).versionCode;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return 0;
	}

	/**
	 * 获取版本名称
	 * 
	 * @return
	 */
	public static String getVersionName() {
		try {
			String str = getActivity().getPackageManager().getPackageInfo(
					getActivity().getPackageName(), 0).versionName;
			return str == null ? "" : str;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return "";
	}
	
	
	
	/**
	 * 检查网络
	 * 
	 * @return
	 */
	public static boolean isConnect() {
		//获取手机所有连接管理对象（包括对wi-fi,net等连接的管理）
		try {
			ConnectivityManager connectivity
				= (ConnectivityManager) getActivity().getSystemService(Context.CONNECTIVITY_SERVICE); 
			if (connectivity != null) {				
				//获取网络连接管理的对象 
				NetworkInfo info = connectivity.getActiveNetworkInfo(); 
				if (info != null&& info.isConnected()) { 
					//判断当前网络是否已经连接 
					if (info.getState() == NetworkInfo.State.CONNECTED) { 
						return true; 
	               } 
	           } 
			} 
		} catch (Exception e) {
		}	
		return false;
	}
	
	/**
	 * 检查网络状态
	 * 
	 * @return
	 */
	public static String checkNetState() {
		try {
			ConnectivityManager conMan = (ConnectivityManager) getActivity()
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			if (conMan.getNetworkInfo(0) == null)
			{
				System.out.println("Platform conMan.getNetworkInfo(0) == null");
				return "";
			}
			NetworkInfo.State mobile = conMan.getNetworkInfo(0).getState();

			boolean wap = isNetworkCMWAPAvailable(getActivity(), CMWAP);
			if (mobile == NetworkInfo.State.CONNECTED
					|| mobile == NetworkInfo.State.CONNECTING) {
				if (wap) {
					return CMWAP;
				} else {
					return CMNET;
				}
			}else{
				System.out.println("Platform mobile ");
			}
			if (conMan.getNetworkInfo(1) == null)
			{
				System.out.println("Platform conMan.getNetworkInfo(1) == null");
				return "";
			}
			NetworkInfo.State wifi = conMan.getNetworkInfo(1).getState();
			if (wifi == NetworkInfo.State.CONNECTED
					|| wifi == NetworkInfo.State.CONNECTING) {
				return WIFI;
			} else {
				System.out.println("Platform not wifi ");
				return "";
			}
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Platform exception ");
		}
		return "";

	}
	
	/**
	 * 获取运营商代码，
	 * 
	 * @return
	 */
	public static int getNetworkOperatorCode() {
		int operatorIntCode = 0;
		try {
			String operatorCode = "0";
			TelephonyManager tm = (TelephonyManager) getActivity()
					.getSystemService(Context.TELEPHONY_SERVICE);
			String netType = checkNetState();
			// operatorCode = tm.getSimOperator();
			String operatorId = tm.getSubscriberId();
			if (operatorId != null) {
				if (operatorId.length() == 5) {
					operatorCode = operatorId;
				} else if (operatorId.length() > 5) {
					operatorCode = operatorId.substring(0, 5);
				}
			}
			if (netType.equals(WIFI)) {
				operatorCode = "9";
			}
			operatorIntCode = Integer.parseInt(operatorCode);
		} catch (Exception e) {
		}

		return operatorIntCode;
	}
	
	private static boolean isNetworkCMWAPAvailable(Activity activity,
			String appName) {
		try {
			Context context = activity.getApplicationContext();
			ConnectivityManager connectivity = (ConnectivityManager) context
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			if (connectivity != null) {
				NetworkInfo[] info = connectivity.getAllNetworkInfo();
				if (info != null) {
					for (int i = 0; i < info.length; i++) {
						String typeName = info[i].getTypeName();
						String extraInfo = info[i].getExtraInfo();
						if (info[i].getState() == NetworkInfo.State.CONNECTED) {
							String subType = info[i].getSubtypeName();
							if (extraInfo != null
									&& extraInfo.contains(appName)) {
								return true;
							} else if (typeName.toLowerCase().equals(appName)) {
								return true;
							}
						}
					}
				}
			}
		} catch (Exception e) {
		}

		return false;
	}

	
	private static String getNetworkName(Activity activity) {
		try {
			Context context = activity.getApplicationContext();
			ConnectivityManager connectivity = (ConnectivityManager) context
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			if (connectivity != null) {
				NetworkInfo[] info = connectivity.getAllNetworkInfo();
				if (info != null) {
					for (int i = 0; i < info.length; i++) {
						String typeName = info[i].getTypeName();
						String extraInfo = info[i].getExtraInfo();
						if (info[i].getState() == NetworkInfo.State.CONNECTED) {
							String subType = info[i].getSubtypeName();
							if (extraInfo != null)
								return extraInfo;
							else
								return typeName
										.toLowerCase(Locale.getDefault());
						}
					}
				}
			}
		} catch (Exception e) {
		}

		return "0";
	}
	
	/**
	 * 获取ip地址
	 * 
	 */
	public static String getIpAddress() {
		WifiInfo wifiInfo = null;
		String ip = "";
		String ipsString = null;
		try {

			WifiManager wifiManager = (WifiManager) getActivity()
					.getSystemService(Context.WIFI_SERVICE);
			wifiInfo = wifiManager.getConnectionInfo();
			int ipAddress = wifiInfo.getIpAddress();
			ip = intToIp(ipAddress);
			String[] ips = ip.split("K");
			
			// 格式化IP地址：xxx.xxx.xxx.xxx
			ipsString = String.format("%03d.%03d.%03d.%03d"
					, Integer.parseInt(ips[0])
					, Integer.parseInt(ips[1])
					, Integer.parseInt(ips[2])
					, Integer.parseInt(ips[3]));
			
		} catch (Exception e) {
		}

		return ipsString == null ? "" : ipsString;

	}
	
	public static String intToIp(int i) {
		// 以“K”作为分隔符
		return (i & 0xFF) + "K" + ((i >> 8) & 0xFF) + "K" + ((i >> 16) & 0xFF)
				+ "K" + (i >> 24 & 0xFF);
	}
	
	/**
	 * 
	 * @return 当前电量的百分比
	 */
	public static int getBatteryStatus() {

		IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		Intent batteryStatus = getActivity().registerReceiver(null, ifilter);
		float level = batteryStatus.getIntExtra("level", 0);
		float scale = batteryStatus.getIntExtra("scale", 100);
		int persent = (int) ((level / scale) * 100);

		return persent;

	}
	
	public static String getIMEI() {
		String imei = "";
		System.out.println("getIMEI=" + imei);
		try {
			TelephonyManager tm = (TelephonyManager) getActivity()
					.getSystemService(Context.TELEPHONY_SERVICE);
			imei = tm.getDeviceId();
		} catch (Exception e) {
			imei = "";
		}
		System.out.println("getIMEI end=" + imei);
		return imei == null ? "" : imei;
	}

	public static String getIMSI() {
		TelephonyManager tm = (TelephonyManager) getActivity()
				.getSystemService(Context.TELEPHONY_SERVICE);
		String imsi = tm.getSubscriberId();
		return imsi;
	}

	public static String getSimOperator() {
		TelephonyManager tm = (TelephonyManager) getActivity()
				.getSystemService(Context.TELEPHONY_SERVICE);
		String imei = tm.getSimOperator();
		return imei;
	}


	
	/**
	 * 获取mac地址
	 * 
	 * @return
	 */
	public static String getMacAddress() {
		return new DefaultMacAddressGetter().getMacAddress(getActivity());
	}
	
	/**
	 * android 默认获取mac地址的方式
	 * 
	 * @author Lewis
	 * 
	 */
	private static class DefaultMacAddressGetter implements IMacAddressGetter {
		private static String macAddress = null;

		@Override
		public String getMacAddress(Activity activity) {
			if (macAddress == null) {
				try {
					WifiManager manager = (WifiManager) activity
							.getSystemService(Context.WIFI_SERVICE);
					WifiInfo wifiInfo = manager.getConnectionInfo();
					macAddress = wifiInfo.getMacAddress();
					if (macAddress != null && macAddress.contains(":")) {
						macAddress = macAddress.replaceAll(":", "")
								.toUpperCase(Locale.getDefault());
					}
				} catch (Exception e) {
					Log.i("Platform","###############getMacAddress catch exception ############");
				}

			}
			Log.i("Platform =", macAddress );
			return macAddress == null ? "" : macAddress;
		}
	}

	/**
	 * mac地址获取
	 * 
	 */
	private static interface IMacAddressGetter {
		/**
		 * mac地址获取
		 */
		String getMacAddress(Activity activity);
	}
}
