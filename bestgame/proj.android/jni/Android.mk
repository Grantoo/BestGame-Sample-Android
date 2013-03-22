LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := bestgame_shared

LOCAL_MODULE_FILENAME := libbestgame

LOCAL_SRC_FILES := bestgame/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GamePayload.cpp \
                   ../../Classes/MainMenuScene.cpp \
                   ../../Classes/StatsScene.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/RulesScene.cpp \
                   ../../Classes/GameHistory.cpp \
                   ../../Classes/JNIBridge.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
