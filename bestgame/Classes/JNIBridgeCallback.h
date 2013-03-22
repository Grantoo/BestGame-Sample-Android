#ifndef __JNIBridgeCallback_H__
#define __JNIBridgeCallback_H__

#include <jni.h>

class JNIBridgeCallback {

public:
	virtual void sdkCompletedWithExit(JNIEnv*, jobject) = 0;
	virtual void sdkCompletedWithMatch(JNIEnv*, jobject, jstring, jstring, jlong, jint, jint) = 0;
	virtual void sdkFailed(JNIEnv*, jobject, jstring, jobject) = 0;

};

#endif // __JNIBridgeCallback_H__
