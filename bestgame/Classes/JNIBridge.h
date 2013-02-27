#ifndef __JNIBridge_H__
#define __JNIBridge_H__

#include <jni.h>
#include "JNIBridgeCallback.h"

class JNIBridge
{

public:

	static void useSandboxServers();
	static void useDebugServers(const char*, const char*, const char*, const char*);
	static void initialize(const char*, const char*);
	static JNIBridge* instance(void);
    static void setNotificationToken(const char*);

	virtual ~JNIBridge();

	void deallocate(JNIEnv*, jobject);

	// methods called from C++ into the bridge
	bool launch(JNIBridgeCallback*);
	bool launchWithTournament(const char*, JNIBridgeCallback*);
	bool launchWithMatchResult(const char*, const char*, long, JNIBridgeCallback*);

    void setOrientation(const char*);

	void syncChallengeCounts();
	int getChallengeCounts();

	// methods called from Java into the bridge
	void sdkCompletedWithExit(JNIEnv*, jobject);
	void sdkCompletedWithMatch(JNIEnv*, jobject, jstring, jstring, jlong, jint, jint);
	void sdkFailed(JNIEnv*, jobject, jstring, jobject);

private:

	static bool getEnv(JNIEnv**);

	JNIBridge();

	JNIBridgeCallback* m_callback;
	jobject m_nativeBridge;

};

#endif // __JNIBridge_H__
