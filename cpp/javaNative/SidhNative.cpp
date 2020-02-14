//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Created by werner on 13.02.20.
// Copyright (c) 2020 Werner Dittmann. All rights reserved.
//

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include <cstdio>

#include "sidh_tests_SidhNativeTests.h"
#include "sidh_SidhNative.h"

/**
 * Define -DPACKAGE_NAME="Java_some_package_name_" to define another package
 * name during compilation
 */
 // Java_sidh_tests_SidhNativeTests_
#ifndef PACKAGE_NAME_TESTS
#define PACKAGE_NAME_TESTS Java_sidh_tests_SidhNativeTests_
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME Java_sidh_SidhNative_
#endif

#ifndef VAR_PREFIX_TEST
#define VAR_PREFIX_TEST sidh_tests_SidhNativeTests_
#endif

#ifndef VAR_PREFIX
#define VAR_PREFIX sidh_SidhNative_
#endif

#define CONCATx(a,b) a##b
#define CONCAT(a,b) CONCATx(a,b)

#define JNI_FUNCTION(FUNC_NAME)  CONCAT(PACKAGE_NAME, FUNC_NAME)
#define JNI_FUNCTION_TESTS(FUNC_NAME)  CONCAT(PACKAGE_NAME_TESTS, FUNC_NAME)

#define VAR_TEST(VAR_NAME)   CONCAT(VAR_PREFIX_TEST, VAR_NAME)
#define VAR(VAR_NAME)   CONCAT(VAR_PREFIX, VAR_NAME)

#ifndef JNIEXPORT
#error "JNIEXPORT not defined"
#endif


static bool useLog = true;
static jobject   loggingCallbackObject = nullptr;
static jmethodID javaLoggingCallback = nullptr;

static JavaVM* javaVM = nullptr;

/**
 * @brief Local helper class to keep track of thread attach / thread detach
 *
 * Used if an underlying implementation runs on an own thread (created an own
 * thread not known to Java). In this case we need to attach this thred to
 * Java runtime before using Java functions, mainly callbacks to Java.
 *
 */
class TrackJNIThread {
    JNIEnv *env;
    bool attached;
public:
    TrackJNIThread() : attached(false), env(nullptr) {

        if (!javaVM) {
#ifdef __ANDROID__
            if (useLog) {
                __android_log_print(ANDROID_LOG_DEBUG, "libsidh", "%s", "TrackJNIThread: javaVM is null, onLoad not called?");
            }
#endif
            return;
        }

        int result = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (result != JNI_OK){
#ifdef __ANDROID__
            result = javaVM->AttachCurrentThread(&env, nullptr);
#else
            result = javaVM->AttachCurrentThread((void**)&env, nullptr);
#endif
            if (!env || result < 0) {
                return;
            }
            attached = true;
        }
    }

    ~TrackJNIThread() {
        if (attached && javaVM)
            javaVM->DetachCurrentThread();
    }

    JNIEnv *getEnv() {
        return env;
    }
};

/**
 * Logging callback, forwards to app logging.
 *
 * "(I[B)V"
 */
static void loggingCallback(int level, const char* data)
{
    TrackJNIThread jni;
    JNIEnv *env = jni.getEnv();
    if (!env)
        return;

    jstring javaString = env->NewStringUTF(data);
    if (loggingCallbackObject != nullptr) {
        env->CallVoidMethod(loggingCallbackObject, javaLoggingCallback, level, javaString);
    }
    if (javaString != nullptr)
        env->DeleteLocalRef(javaString);
}

#ifdef __cplusplus
extern "C" {
#endif
void Log(char const *format, ...) {
    char buffer[1000];
    va_list arg;
    va_start(arg, format);
    vsnprintf (buffer, 999, format, arg);
    va_end(arg);
    if (javaLoggingCallback != nullptr) {
        loggingCallback(4, buffer);
    } else {
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_DEBUG, "libsidh", "%s", buffer);
#else
        printf("%s", buffer);
#endif
    }
}

#ifdef ADD_TESTS
#include "../testsEmbedded/embeddedTests.h"
int arithTests434(enum RunTestBits runTestBits);
int arithTests503(enum RunTestBits runTestBits);
int arithTests610(enum RunTestBits runTestBits);
int arithTests751(enum RunTestBits runTestBits);

int dhTestsp434();
int dhTestsp503();
int dhTestsp610();
int dhTestsp751();

#endif

#ifdef __cplusplus
}
#endif

#ifdef ADD_TESTS
/*
 * Class:     sidh_tests_SidhNativeTests
 * Method:    runSidhTests
 * Signature: (II)Z
 */
JNIEXPORT jboolean
JNI_FUNCTION_TESTS(runSidhTests)(JNIEnv *env, jclass clazz, jint sidhType, jint testMask)
{
    (void)env;
    (void)clazz;

    bool OK = true;

    switch (sidhType) {
        case VAR(P434):
            OK = OK && arithTests434((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp434() == 0);
            }
            return OK;

        case VAR(P503):
            OK = OK && arithTests503((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp503() == 0);
            }
            return OK;

        case VAR(P610):
            OK = OK && arithTests610((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp610() == 0);
            }
            return OK;

        case VAR(P751):
            OK = OK && arithTests751((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp751() == 0);
            }
            return OK;

        default:
            break;
    }
    return false;
}

#endif
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    (void)reserved;
    javaVM = vm;
    return JNI_VERSION_1_6;
}
#pragma clang diagnostic pop

/*
 * Class:     sidh_tests_SidhNativeTests
 * Method:    enableLoggingCallback
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
JNI_FUNCTION(enableLoggingCallback)(JNIEnv * env, jobject thiz) {

    // Initialize the callbacks etc which point back to Java runtime
    if (loggingCallbackObject == nullptr) {
        loggingCallbackObject = env->NewGlobalRef(thiz);
        if (loggingCallbackObject == nullptr) {
            return -1;
        }
        jclass callbackClass = nullptr;
        callbackClass = env->GetObjectClass(loggingCallbackObject);
        if (callbackClass == nullptr) {
            env->DeleteGlobalRef(loggingCallbackObject);
            return -2;
        }
        javaLoggingCallback = env->GetMethodID(callbackClass, "loggingCallback", "(ILjava/lang/String;)V");
        if (javaLoggingCallback == nullptr) {
            env->DeleteGlobalRef(loggingCallbackObject);
            return -3;
        }
    }
    return 1;
}

/*
 * Class:     sidh_SidhNative
 * Method:    disableLoggingCallback
 * Signature: ()I
 */
JNIEXPORT void JNICALL
JNI_FUNCTION(disableLoggingCallback)(JNIEnv * env, jclass clazz) {
    if (loggingCallbackObject != nullptr) {
        env->DeleteGlobalRef(loggingCallbackObject);
    }
    loggingCallbackObject = nullptr;
    javaLoggingCallback = nullptr;
}
