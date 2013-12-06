#ifndef __JNIBridge_H__
#define __JNIBridge_H__

#include <jni.h>
#include "JNIBridgeCallback.h"
#include "JNIBridgeBroadcastReceiver.h"

class JNIBridge
{

public:

	static void useSandbox();
	static void initialize(const char*, const char*);
	static JNIBridge* instance(void);

	virtual ~JNIBridge();

	void deallocate(JNIEnv*, jobject);

    void setOrientation(const char*);

	// launch methods called from C++ into the bridge
	bool launch(JNIBridgeCallback*);
	bool launchWithMatchResult(const char*, const char*, long, JNIBridgeCallback*);

	// callback methods called from Java into the bridge
	void sdkCompletedWithExit(JNIEnv*, jobject);
	void sdkCompletedWithMatch(JNIEnv*, jobject, jstring, jstring, jlong, jint, jint);
	void sdkFailed(JNIEnv*, jobject, jstring, jobject);

	// sync methods called from C++ into the bridge
	void syncChallengeCounts(JNIBridgeBroadcastReceiver*);
	void syncTournamentInfo(JNIBridgeBroadcastReceiver*);

	// broadcast methods called from Java into the bridge
	void updatedChallengeCount(JNIEnv*, jobject, jint);
	void updatedTournamentInfo(JNIEnv*, jobject, jstring, jstring, jstring, jlong, jlong, jstring);

private:

	static bool getEnv(JNIEnv**);

	JNIBridge();

	JNIBridgeCallback* m_callback;
	JNIBridgeBroadcastReceiver* m_broadcastReceiver;
	jobject m_nativeBridge;

};

#endif // __JNIBridge_H__
