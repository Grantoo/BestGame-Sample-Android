#ifndef __JNIBridgeBroadcastReceiver_H__
#define __JNIBridgeBroadcastReceiver_H__

#include <jni.h>

class JNIBridgeBroadcastReceiver {

public:

	virtual void updatedChallengeCount(JNIEnv*, jobject, jint) = 0;
	virtual void updatedTournamentInfo(JNIEnv*, jobject, jstring, jstring, jstring, jlong, jlong, jstring) = 0;

};

#endif // __JNIBridgeBroadcastReceiver_H__
