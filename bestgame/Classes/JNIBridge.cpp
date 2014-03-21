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

void Java_org_grantoo_bestgame_NativeBridge_nativeUpdatedChallengeCount(JNIEnv* env, jobject thiz, jint count) {
	JNIBridge::instance()->updatedChallengeCount(env, thiz, count);
}

void Java_org_grantoo_bestgame_NativeBridge_nativeUpdatedTournamentInfo(JNIEnv* env, jobject thiz, jstring name, jstring campaignName, jstring sponsorName, jlong startDate, jlong endDate, jstring logo) {
	JNIBridge::instance()->updatedTournamentInfo(env, thiz, name, campaignName, sponsorName, startDate, endDate, logo);
}

bool Java_org_grantoo_bestgame_NativeBridge_nativeSdkSocialLogin(JNIEnv* env, jobject thiz, jboolean allowCache) {
    return JNIBridge::instance()->sdkSocialLogin(env, thiz, allowCache);
}

bool Java_org_grantoo_bestgame_NativeBridge_nativeSdkSocialInvite(JNIEnv* env, jobject thiz, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
    return JNIBridge::instance()->sdkSocialInvite(env, thiz, subject, longMessage, shortMessage, linkUrl);
}

bool Java_org_grantoo_bestgame_NativeBridge_nativeSdkSocialShare(JNIEnv* env, jobject thiz, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
    return JNIBridge::instance()->sdkSocialShare(env, thiz, subject, longMessage, shortMessage, linkUrl);
}

void Java_org_grantoo_bestgame_NativeBridge_nativeUpdatedVirtualGoods(JNIEnv* env, jobject thiz, jstring transactionId, jobjectArray virtualGoods) {
    JNIBridge::instance()->updatedVirtualGoods(env, thiz, transactionId, virtualGoods);
}

void Java_org_grantoo_bestgame_NativeBridge_nativeRollbackVirtualGoods(JNIEnv* env, jobject thiz, jstring transactionId) {
    JNIBridge::instance()->rollbackVirtualGoods(env, thiz, transactionId);
}

}

static JNIBridge* s_instance = 0;

#pragma mark -
#pragma mark Public methods

void JNIBridge::useSandbox() {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"useSandbox",
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

void JNIBridge::initialize(const char* gameId, const char* gameSecret, bool gameHasLogin, bool gameHasInvite, bool gameHasShare) {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"initialize",
		"(Ljava/lang/String;Ljava/lang/String;ZZZ)V")) {
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
    jboolean jGameHasLogin = (jboolean) gameHasLogin;
    jboolean jGameHasInvite = (jboolean) gameHasInvite;
    jboolean jGameHasShare = (jboolean) gameHasShare;

    jniMethodInfo.env->CallStaticVoidMethod(
		jniMethodInfo.classID,
		jniMethodInfo.methodID,
		jGameId,
		jGameSecret,
		jGameHasLogin,
		jGameHasInvite,
		jGameHasShare);

    env->DeleteLocalRef(jGameId);
    env->DeleteLocalRef(jGameSecret);

    if (jAuxData) {
    	env->DeleteLocalRef(jAuxData);
    }

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

bool JNIBridge::sdkSocialLogin(JNIEnv* env, jobject context, jboolean allowCache) {
	if (m_callback) {
		return m_callback->sdkSocialLogin(env, context, allowCache);
	}

	return false;
}

bool JNIBridge::sdkSocialInvite(JNIEnv* env, jobject context, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
	if (m_callback) {
		return m_callback->sdkSocialInvite(env, context, subject, longMessage, shortMessage, linkUrl);
	}

	return false;
}

bool JNIBridge::sdkSocialShare(JNIEnv* env, jobject context, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
	if (m_callback) {
		return m_callback->sdkSocialShare(env, context, subject, longMessage, shortMessage, linkUrl);
	}

	return false;
}

bool JNIBridge::sdkSocialLoginCompleted(const char* result) {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
        jniMethodInfo,
        "org/grantoo/bestgame/NativeBridge",
        "sdkSocialLoginCompleted",
        "(Ljava/lang/String;)Z")) {
        // method info could not be obtained
        return false;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
        // unable to get the environment
        return false;
    }

    jstring jResult = NULL;

    if (result) {
    	jResult = env->NewStringUTF(result);
    }

    jboolean methodResult = jniMethodInfo.env->CallBooleanMethod(
        m_nativeBridge,
        jniMethodInfo.methodID,
        jResult);

    if (jResult) {
    	env->DeleteLocalRef(jResult);
    }

    if (jniMethodInfo.env->ExceptionCheck()) {
        // exception thrown in the called method
        return false;
    }

    return (bool) methodResult;
}

bool JNIBridge::sdkSocialInviteCompleted() {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
        jniMethodInfo,
        "org/grantoo/bestgame/NativeBridge",
        "sdkSocialInviteCompleted",
        "()Z")) {
        // method info could not be obtained
        return false;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
        // unable to get the environment
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

bool JNIBridge::sdkSocialShareCompleted() {
    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
        jniMethodInfo,
        "org/grantoo/bestgame/NativeBridge",
        "sdkSocialShareCompleted",
        "()Z")) {
        // method info could not be obtained
        return false;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
        // unable to get the environment
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

void JNIBridge::updatedChallengeCount(JNIEnv* env, jobject context, jint count) {
	if (m_broadcastReceiver) {
		m_broadcastReceiver->updatedChallengeCount(env, context, count);
	}
}

void JNIBridge::syncChallengeCounts(JNIBridgeBroadcastReceiver* broadcastReceiver) {
	m_broadcastReceiver = broadcastReceiver;

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

void JNIBridge::syncTournamentInfo(JNIBridgeBroadcastReceiver* broadcastReceiver) {
	m_broadcastReceiver = broadcastReceiver;

    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
		jniMethodInfo,
		"org/grantoo/bestgame/NativeBridge",
		"syncTournamentInfo",
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

void JNIBridge::updatedTournamentInfo(JNIEnv* env, jobject context, jstring name, jstring campaignName, jstring sponsorName, jlong startDate, jlong endDate, jstring logo) {
	if (m_broadcastReceiver) {
		m_broadcastReceiver->updatedTournamentInfo(env, context, name, campaignName, sponsorName, startDate, endDate, logo);
	}
}

bool JNIBridge::syncVirtualGoods(JNIBridgeBroadcastReceiver* broadcastReceiver) {
    m_broadcastReceiver = broadcastReceiver;

    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
        jniMethodInfo,
        "org/grantoo/bestgame/NativeBridge",
        "syncVirtualGoods",
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

bool JNIBridge::acknowledgeVirtualGoods(JNIBridgeBroadcastReceiver* broadcastReceiver, const char* transactionId, bool consumed) {
    m_broadcastReceiver = broadcastReceiver;

    JniMethodInfo jniMethodInfo;

    if (!JniHelper::getMethodInfo(
        jniMethodInfo,
        "org/grantoo/bestgame/NativeBridge",
        "acknowledgeVirtualGoods",
        "(Ljava/lang/String;Z)Z")) {
        // method info could not be obtained
        return false;
    }

    JNIEnv* env = 0;

    if (!getEnv(&env)) {
        // unable to get the environment
        return false;
    }

    jstring jTransId = env->NewStringUTF(transactionId);
    jboolean jConsumed = (jboolean) consumed;

    jboolean methodResult = jniMethodInfo.env->CallBooleanMethod(
        m_nativeBridge,
        jniMethodInfo.methodID,
        jTransId,
        jConsumed);

    env->DeleteLocalRef(jTransId);

    if (jniMethodInfo.env->ExceptionCheck()) {
        // exception thrown in the called method
        return false;
    }

    return (bool) methodResult;
}

void JNIBridge::updatedVirtualGoods(JNIEnv* env, jobject context, jstring transactionId, jobjectArray virtualGoods) {
    if (m_broadcastReceiver) {
        m_broadcastReceiver->updatedVirtualGoods(env, context, transactionId, virtualGoods);
    }
}

void JNIBridge::rollbackVirtualGoods(JNIEnv* env, jobject context, jstring transactionId) {
    if (m_broadcastReceiver) {
        m_broadcastReceiver->rollbackVirtualGoods(env, context, transactionId);
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
	m_broadcastReceiver = NULL;

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
