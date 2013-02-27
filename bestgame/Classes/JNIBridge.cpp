#include "JNIBridge.h"
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"

using namespace cocos2d;

#define LOG_TAG		"JNIBridge"
#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

extern "C"
{

void Java_org_grantoo_bestgame_NativeBridge_deallocateJNIBridge(JNIEnv* env, jobject thiz) {
	JNIBridge::instance()->deallocate(env, thiz);
}

void Java_org_grantoo_bestgame_NativeBridge_nativeSdkCompletedWithExit(JNIEnv* env, jobject thiz) {
	JNIBridge::instance()->sdkCompletedWithExit(env, thiz);
}

void Java_org_grantoo_bestgame_NativeBridge_nativeSdkCompletedWithMatch(JNIEnv* env, jobject thiz, jstring tournId, jstring matchId, jlong seed, jint round, jint gameType) {
	JNIBridge::instance()->sdkCompletedWithMatch(env, thiz, tournId, matchId, seed, round, gameType);
}

void Java_org_grantoo_bestgame_NativeBridge_nativeSdkFailed(JNIEnv* env, jobject thiz, jstring message, jobject result) {
	JNIBridge::instance()->sdkFailed(env, thiz, message, result);
}

}

static JNIBridge* s_instance = 0;

#pragma mark -
#pragma mark Public methods

void JNIBridge::useSandboxServers() {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"useSandboxServers",
		"()V")) {
    	// method info could not be obtained
    	return;
    }

    jniMethodInfo.env->CallStaticVoidMethod(
		jniMethodInfo.classID,
		jniMethodInfo.methodID);


    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return;
    }

    return;
}

void JNIBridge::useDebugServers(const char* serverUrl, const char* grantooAPI, const char* tournamentAPI, const char* challengeAPI) {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"useDebugServers",
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
    	// method info could not be obtained
    	return;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
    	// unable to get the environment
    	return;
    }

    jstring jServerUrl = env->NewStringUTF(serverUrl);
    jstring jGrantooAPI = env->NewStringUTF(grantooAPI);
    jstring jTournamentAPI = env->NewStringUTF(tournamentAPI);
    jstring jChallengeAPI = env->NewStringUTF(challengeAPI);

    jniMethodInfo.env->CallStaticVoidMethod(
		jniMethodInfo.classID,
		jniMethodInfo.methodID,
		jServerUrl,
		jGrantooAPI,
		jTournamentAPI,
		jChallengeAPI);

    env->DeleteLocalRef(jServerUrl);
    env->DeleteLocalRef(jGrantooAPI);
    env->DeleteLocalRef(jTournamentAPI);
    env->DeleteLocalRef(jChallengeAPI);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return;
    }

    return;
}

void JNIBridge::initialize(const char* gameId, const char* gameSecret) {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"initialize",
		"(Ljava/lang/String;Ljava/lang/String;)V")) {
    	// method info could not be obtained
    	return;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
    	// unable to get the environment
    	return;
    }

    jstring jGameId = env->NewStringUTF(gameId);
    jstring jGameSecret = env->NewStringUTF(gameSecret);

    jniMethodInfo.env->CallStaticVoidMethod(
		jniMethodInfo.classID,
		jniMethodInfo.methodID,
		jGameId,
		jGameSecret);

    env->DeleteLocalRef(jGameId);
    env->DeleteLocalRef(jGameSecret);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return;
    }

    return;
}

void JNIBridge::setNotificationToken(const char* token) {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"setNotificationToken",
		"(Ljava/lang/String;)V")) {
    	// method info could not be obtained
    	return;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
    	// unable to get the environment
    	return;
    }

    jstring jToken= env->NewStringUTF(token);

    jniMethodInfo.env->CallStaticVoidMethod(
		jniMethodInfo.classID,
		jniMethodInfo.methodID,
		jToken);

    env->DeleteLocalRef(jToken);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return;
    }
    return;
}

JNIBridge* JNIBridge::instance(void) {
    if (!s_instance) {
        s_instance = new JNIBridge();
    }

    return s_instance;
}

JNIBridge::~JNIBridge() {
}

void JNIBridge::deallocate(JNIEnv* env, jobject thiz) {
	env->DeleteGlobalRef(m_nativeBridge);
	m_nativeBridge = NULL;
}

void JNIBridge::setOrientation(const char* orientation) {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"setOrientation",
		"(Ljava/lang/String;)V")) {
    	// method info could not be obtained
    	return;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
    	// unable to get the environment
    	return;
    }

    jstring jOrientation = env->NewStringUTF(orientation);

    jniMethodInfo.env->CallVoidMethod(
		m_nativeBridge,
		jniMethodInfo.methodID,
		jOrientation);

    env->DeleteLocalRef(jOrientation);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return;
    }
    return;
}

bool JNIBridge::launch(JNIBridgeCallback* callback) {
    m_callback = callback;

    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"launch",
		"()Z")) {
    	// method info could not be obtained
    	return false;
    }

    jboolean methodResult = jniMethodInfo.env->CallBooleanMethod(
		m_nativeBridge,
		jniMethodInfo.methodID);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return false;
    }

    return (bool) methodResult;
}

bool JNIBridge::launchWithTournament(const char* tournId, JNIBridgeCallback* callback) {
    m_callback = callback;

    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"launchWithTournament",
		"(Ljava/lang/String;)Z")) {
    	// method info could not be obtained
    	return false;
    }

    jboolean methodResult = jniMethodInfo.env->CallBooleanMethod(
		m_nativeBridge,
		jniMethodInfo.methodID);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return false;
    }

    return (bool) methodResult;
}

bool JNIBridge::launchWithMatchResult(const char* tournId, const char* matchId, long score, JNIBridgeCallback* callback) {
    m_callback = callback;

    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"launchWithMatchResult",
		"(Ljava/lang/String;Ljava/lang/String;J)Z")) {
    	// method info could not be obtained
    	return false;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
    	// unable to get the environment
    	return false;
    }

    jstring jTournId = env->NewStringUTF(tournId);
    jstring jMatchId = env->NewStringUTF(matchId);
    jlong jScore = (jlong) score;

    jboolean methodResult = jniMethodInfo.env->CallBooleanMethod(
		m_nativeBridge,
		jniMethodInfo.methodID,
		jTournId,
		jMatchId,
		jScore);

    env->DeleteLocalRef(jTournId);
    env->DeleteLocalRef(jMatchId);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return false;
    }

    return (bool) methodResult;
}

void JNIBridge::syncChallengeCounts() {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"syncChallengeCounts",
		"()V")) {
    	// method info could not be obtained
    	return;
    }

    jniMethodInfo.env->CallVoidMethod(
		m_nativeBridge,
		jniMethodInfo.methodID);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return;
    }

    return;
}

int JNIBridge::getChallengeCounts() {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"getChallengeCounts",
		"()I")) {
    	// method info could not be obtained
    	return 0;
    }

    jint methodResult = jniMethodInfo.env->CallIntMethod(
		m_nativeBridge,
		jniMethodInfo.methodID);

    if (jniMethodInfo.env->ExceptionCheck()) {
    	// exception thrown in the called method
    	return 0;
    }

    return (int) methodResult;
}

void JNIBridge::sdkCompletedWithExit(JNIEnv* env, jobject context) {
	if (m_callback) {
		m_callback->sdkCompletedWithExit(env, context);
	}
}

void JNIBridge::sdkCompletedWithMatch(JNIEnv* env, jobject context, jstring tournId, jstring matchId, jlong seed, jint round, jint gameType) {
	if (m_callback) {
		m_callback->sdkCompletedWithMatch(env, context, tournId, matchId, seed, round, gameType);
	}
}

void JNIBridge::sdkFailed(JNIEnv* env, jobject context, jstring message, jobject result) {
	if (m_callback) {
		m_callback->sdkFailed(env, context, message, result);
	}
}

#pragma mark -
#pragma mark Private methods

bool JNIBridge::getEnv(JNIEnv** env) {
	if (JniHelper::getJavaVM()->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK) {
		LOGD("Failed to get the environment using GetEnv()");
		return false;
	}

	if (JniHelper::getJavaVM()->AttachCurrentThread(env, 0) < 0) {
		LOGD("Failed to get the environment using AttachCurrentThread()");
		return false;
	}

	return true;
}

JNIBridge::JNIBridge() {
	m_callback = NULL;

	JNIEnv* env = 0;

	if (!getEnv(&env)) {
		// unable to get env
		return;
	}

	JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"<init>",
		"()V")) {
    	// method info could not be obtained
    	return;
    }

    jobject nativeBridge = env->NewObject(
			jniMethodInfo.classID,
			jniMethodInfo.methodID);

    m_nativeBridge = env->NewGlobalRef(nativeBridge);
}
