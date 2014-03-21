#ifndef __JNIBridgeCallback_H__
#define __JNIBridgeCallback_H__

#include <jni.h>

class JNIBridgeCallback {

public:

	virtual void sdkCompletedWithExit(JNIEnv*, jobject) = 0;
	virtual void sdkCompletedWithMatch(JNIEnv*, jobject, jstring, jstring, jlong, jint, jint) = 0;
	virtual void sdkFailed(JNIEnv*, jobject, jstring, jobject) = 0;

    virtual bool sdkSocialLogin(JNIEnv* env, jobject context, jboolean allowCache) {
    	// by default do nothing
    	return false;
    }

    virtual bool sdkSocialInvite(JNIEnv* env, jobject context, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
    	// by default do nothing
    	return false;
    }

    virtual bool sdkSocialShare(JNIEnv* env, jobject context, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
    	// by default do nothing
    	return false;
    }

};

#endif // __JNIBridgeCallback_H__
