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
#include <cstdint>

#include "sidhjava_test_SidhNativeTests.h"
#include "sidhjava_SidhNative.h"
#include "../SidhWrapper.h"

/**
 * Define -DPACKAGE_NAME="Java_some_package_name_" to define another package
 * name during compilation
 */
 // Java_sidh_tests_SidhNativeTests_
#ifndef PACKAGE_NAME_TESTS
#define PACKAGE_NAME_TESTS Java_sidhjava_test_SidhNativeTests_
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME Java_sidhjava_SidhNative_
#endif

#ifndef VAR_PREFIX_TEST
#define VAR_PREFIX_TEST sidhjava_test_SidhNativeTests_
#endif

#ifndef VAR_PREFIX
#define VAR_PREFIX sidhjava_SidhNative_
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

static jclass    fieldLengthsClass = nullptr;
static jmethodID fieldLengthsConsID = nullptr;
static jfieldID  privateKeyAID = nullptr;
static jfieldID  privateKeyBID = nullptr;
static jfieldID  publicKeyID = nullptr;
static jfieldID  sharedSecretID = nullptr;

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
    JNIEnv *env = nullptr;
    bool attached = false;
public:
    TrackJNIThread() {

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

// Don't force a flush on stdout when not running/including testing code
#ifndef FLUSH
#define FLUSH
#endif

static void clearGlobalReferences(JNIEnv *env) {
    if (fieldLengthsClass) {
        env->DeleteGlobalRef(fieldLengthsClass);
    }
    fieldLengthsConsID = nullptr;
    privateKeyAID = nullptr;
    privateKeyBID = nullptr;
    publicKeyID = nullptr;
    sharedSecretID = nullptr;
}

static int32_t initializeFieldLengthsInfo(JNIEnv * env) {
    // Prepare access to the FieldLengths Java class.
    jclass tempClassRef = env->FindClass( "sidhjava/SidhNative$FieldLengths" );
    if (tempClassRef == nullptr) {
        clearGlobalReferences(env);
        return -1;
    }
    fieldLengthsClass = reinterpret_cast<jclass>(env->NewGlobalRef(tempClassRef));

    privateKeyAID = env->GetFieldID(fieldLengthsClass, "privateKeyA", "J");
    if (privateKeyAID == nullptr) {
        clearGlobalReferences(env);
        return -2;
    }

    privateKeyBID = env->GetFieldID(fieldLengthsClass, "privateKeyB", "J");
    if (privateKeyBID == nullptr) {
        clearGlobalReferences(env);
        return -3;
    }

    publicKeyID = env->GetFieldID(fieldLengthsClass, "publicKey", "J");
    if (publicKeyID == nullptr) {
        clearGlobalReferences(env);
        return -4;
    }

    sharedSecretID = env->GetFieldID(fieldLengthsClass, "sharedSecret", "J");
    if (sharedSecretID == nullptr) {
        clearGlobalReferences(env);
        return -5;
    }

    fieldLengthsConsID = env->GetMethodID(fieldLengthsClass, "<init>", "()V");
    if (fieldLengthsConsID == nullptr) {
        clearGlobalReferences(env);
        return -6;
    }
    return 1;
}


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

    bool OK;

    switch (sidhType) {
        case VAR(P434):
            OK = arithTests434((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp434() == 0);
            }
            return (jboolean)OK;

        case VAR(P503):
            OK = arithTests503((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp503() == 0);
            }
            return (jboolean)OK;

        case VAR(P610):
            OK = arithTests610((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp610() == 0);
            }
            return (jboolean)OK;

        case VAR(P751):
            OK = arithTests751((RunTestBits)testMask);
            if (testMask & VAR_TEST(DiffieHellman)) {
                OK = OK && (dhTestsp751() == 0);
            }
            return (jboolean)OK;

        default:
            break;
    }
    return (jboolean) false;
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
        jclass callbackClass = env->GetObjectClass(loggingCallbackObject);
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
    (void)clazz;

    if (loggingCallbackObject != nullptr) {
        env->DeleteGlobalRef(loggingCallbackObject);
    }
    loggingCallbackObject = nullptr;
    javaLoggingCallback = nullptr;
}

/*
 * Class:     sidh_SidhNative
 * Method:    getFieldLengths
 * Signature: (I)Lsidh/SidhNative/FieldLengths;
 */
JNIEXPORT jobject JNICALL
JNI_FUNCTION(getFieldLengths)(JNIEnv * env, jclass clazz, jint sidhType) {

    (void) clazz;

    if (sidhType < SidhWrapper::P434 || sidhType > SidhWrapper::P751Comp) return nullptr;

    if (fieldLengthsClass == nullptr) {
        auto result = initializeFieldLengthsInfo(env);
        if (result < 0) {
            Log("Cannot setup field length class, code: %d", result);
            return nullptr;
        }
    }
    jobject lengthsDataJava = nullptr;
    auto lengthPtr = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    if (lengthPtr) {
        lengthsDataJava = env->NewObject(fieldLengthsClass, fieldLengthsConsID);
        env->SetLongField(lengthsDataJava, privateKeyAID, static_cast<jlong>(lengthPtr->privateKeyA));
        env->SetLongField(lengthsDataJava, privateKeyBID, static_cast<jlong>(lengthPtr->privateKeyB));
        env->SetLongField(lengthsDataJava, publicKeyID, static_cast<jlong>(lengthPtr->publicKey));
        env->SetLongField(lengthsDataJava, sharedSecretID, static_cast<jlong>(lengthPtr->sharedSecret));
    }
    return lengthsDataJava;
}

/*
 * Class:     sidhjava_SidhNative
 * Method:    randomModOrderA
 * Signature: (I[B)Z
 */
JNIEXPORT jboolean JNICALL
JNI_FUNCTION(randomModOrderA)(JNIEnv *env, jclass clazz, jint sidhType, jbyteArray randomOut) {

    (void)clazz;

    auto lengths = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    auto dataLen = static_cast<size_t>(env->GetArrayLength(randomOut));
    if (lengths->privateKeyA != dataLen) {
        Log("%s: output field length %d does not match required length %d", __func__, dataLen, lengths->privateKeyA);
        FLUSH
        return (jboolean) false;
    }
    auto * tmp = (uint8_t *)env->GetByteArrayElements(randomOut, nullptr);
    SidhWrapper::random_mod_order_A(static_cast<SidhWrapper::SidhType>(sidhType), tmp);

    env->ReleaseByteArrayElements(randomOut, (jbyte *)tmp, 0);
    return (jboolean) true;
}

/*
 * Class:     sidhjava_SidhNative
 * Method:    randomModOrderB
 * Signature: (I[B)Z
 */
JNIEXPORT jboolean JNICALL
JNI_FUNCTION(randomModOrderB)(JNIEnv *env, jclass clazz, jint sidhType, jbyteArray randomOut) {
    (void)clazz;

    auto lengths = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    auto dataLen = static_cast<size_t>(env->GetArrayLength(randomOut));
    if (lengths->privateKeyB != dataLen) {
        Log("%s: output field length %d does not match required length %d", __func__, dataLen, lengths->privateKeyB);
        FLUSH
        return (jboolean) false;
    }
    auto * tmp = (uint8_t *)env->GetByteArrayElements(randomOut, nullptr);
    SidhWrapper::random_mod_order_A(static_cast<SidhWrapper::SidhType>(sidhType), tmp);

    env->ReleaseByteArrayElements(randomOut, (jbyte *)tmp, 0);
    return (jboolean)true;
}

/*
 * Class:     sidhjava_SidhNative
 * Method:    ephemeralKeyGenerationA
 * Signature: (I[B[B)Z
 */
JNIEXPORT jboolean JNICALL
JNI_FUNCTION(ephemeralKeyGenerationA)(JNIEnv *env, jclass clazz, jint sidhType, jbyteArray privateKeyA, jbyteArray publicKeyA) {
    (void)clazz;

    auto lengths = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    auto privateKeyLen = static_cast<size_t>(env->GetArrayLength(privateKeyA));
    auto publicKeyLen = static_cast<size_t>(env->GetArrayLength(publicKeyA));

    if (lengths->privateKeyA != privateKeyLen || lengths->publicKey != publicKeyLen) {
        Log("%s: field lengths (%d, %d) do not match required length: (%d, %d)",
                __func__, privateKeyLen, publicKeyLen, lengths->privateKeyA, lengths->publicKey);
        FLUSH
        return (jboolean) false;
    }
    auto * privateTmp = (uint8_t *)env->GetByteArrayElements(privateKeyA, nullptr);
    auto * publicTmp = (uint8_t *)env->GetByteArrayElements(publicKeyA, nullptr);

    auto result = SidhWrapper::EphemeralKeyGeneration_A(static_cast<SidhWrapper::SidhType>(sidhType), privateTmp, publicTmp);

    env->ReleaseByteArrayElements(privateKeyA, (jbyte *)privateTmp, 0);
    env->ReleaseByteArrayElements(publicKeyA, (jbyte *)publicTmp, 0);
    return (jboolean) result;
}

/*
 * Class:     sidhjava_SidhNative
 * Method:    ephemeralKeyGenerationB
 * Signature: (I[B[B)Z
 */
JNIEXPORT jboolean JNICALL
JNI_FUNCTION(ephemeralKeyGenerationB)(JNIEnv *env, jclass clazz, jint sidhType, jbyteArray privateKeyB, jbyteArray publicKeyB) {
    (void)clazz;

    auto lengths = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    auto privateKeyLen = static_cast<size_t>(env->GetArrayLength(privateKeyB));
    auto publicKeyLen = static_cast<size_t>(env->GetArrayLength(publicKeyB));

    if (lengths->privateKeyB != privateKeyLen || lengths->publicKey != publicKeyLen) {
        Log("%s: field lengths (%d, %d) do not match required length: (%d, %d)",
            __func__, privateKeyLen, publicKeyLen, lengths->privateKeyA, lengths->publicKey);
        FLUSH
        return (jboolean) false;
    }
    auto * privateTmp = (uint8_t *)env->GetByteArrayElements(privateKeyB, nullptr);
    auto * publicTmp = (uint8_t *)env->GetByteArrayElements(publicKeyB, nullptr);

    auto result = SidhWrapper::EphemeralKeyGeneration_B(static_cast<SidhWrapper::SidhType>(sidhType), privateTmp, publicTmp);

    env->ReleaseByteArrayElements(privateKeyB, (jbyte *)privateTmp, 0);
    env->ReleaseByteArrayElements(publicKeyB, (jbyte *)publicTmp, 0);
    return (jboolean) result;
}

/*
 * Class:     sidhjava_SidhNative
 * Method:    ephemeralSecretAgreementA
 * Signature: (I[B[B[B)Z
 */
JNIEXPORT jboolean JNICALL
JNI_FUNCTION(ephemeralSecretAgreementA)(JNIEnv *env, jclass clazz, jint sidhType, jbyteArray privateKeyA, jbyteArray publicKeyB, jbyteArray sharedSecret) {
    (void)clazz;

    auto lengths = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    auto privateKeyLen = static_cast<size_t>(env->GetArrayLength(privateKeyA));
    auto publicKeyLen = static_cast<size_t>(env->GetArrayLength(publicKeyB));
    auto sharedSecretLen = static_cast<size_t>(env->GetArrayLength(sharedSecret));

    if (lengths->privateKeyA != privateKeyLen || lengths->publicKey != publicKeyLen || lengths->sharedSecret != sharedSecretLen) {
        Log("%s: field lengths (%d, %d, %d) do not match required length: (%d, %d, %d)",
            __func__, privateKeyLen, publicKeyLen, sharedSecretLen, lengths->privateKeyA, lengths->publicKey, lengths->sharedSecret);
        FLUSH
        return (jboolean) false;
    }
    auto * privateTmp = (uint8_t *)env->GetByteArrayElements(privateKeyA, nullptr);
    auto * publicTmp = (uint8_t *)env->GetByteArrayElements(publicKeyB, nullptr);
    auto * sharedTmp = (uint8_t *)env->GetByteArrayElements(sharedSecret, nullptr);

    auto result = SidhWrapper::EphemeralSecretAgreement_A(static_cast<SidhWrapper::SidhType>(sidhType), privateTmp, publicTmp, sharedTmp);

    env->ReleaseByteArrayElements(privateKeyA, (jbyte *)privateTmp, 0);
    env->ReleaseByteArrayElements(publicKeyB, (jbyte *)publicTmp, 0);
    env->ReleaseByteArrayElements(sharedSecret, (jbyte *)sharedTmp, 0);
    return (jboolean) result;
}

/*
 * Class:     sidhjava_SidhNative
 * Method:    ephemeralSecretAgreementB
 * Signature: (I[B[B[B)Z
 */
JNIEXPORT jboolean JNICALL
JNI_FUNCTION(ephemeralSecretAgreementB)(JNIEnv *env, jclass clazz, jint sidhType, jbyteArray privateKeyB, jbyteArray publicKeyA, jbyteArray sharedSecret) {
    (void)clazz;

    auto lengths = SidhWrapper::getFieldLengths(static_cast<SidhWrapper::SidhType>(sidhType));
    auto privateKeyLen = static_cast<size_t>(env->GetArrayLength(privateKeyB));
    auto publicKeyLen = static_cast<size_t>(env->GetArrayLength(publicKeyA));
    auto sharedSecretLen = static_cast<size_t>(env->GetArrayLength(sharedSecret));

    if (lengths->privateKeyB != privateKeyLen || lengths->publicKey != publicKeyLen || lengths->sharedSecret != sharedSecretLen) {
        Log("%s: field lengths (%d, %d, %d) do not match required length: (%d, %d, %d)",
            __func__, privateKeyLen, publicKeyLen, sharedSecretLen, lengths->privateKeyA, lengths->publicKey, lengths->sharedSecret);
        FLUSH
        return (jboolean) false;
    }
    auto * privateTmp = (uint8_t *)env->GetByteArrayElements(privateKeyB, nullptr);
    auto * publicTmp = (uint8_t *)env->GetByteArrayElements(publicKeyA, nullptr);
    auto * sharedTmp = (uint8_t *)env->GetByteArrayElements(sharedSecret, nullptr);

    auto result = SidhWrapper::EphemeralSecretAgreement_B(static_cast<SidhWrapper::SidhType>(sidhType), privateTmp, publicTmp, sharedTmp);

    env->ReleaseByteArrayElements(privateKeyB, (jbyte *)privateTmp, 0);
    env->ReleaseByteArrayElements(publicKeyA, (jbyte *)publicTmp, 0);
    env->ReleaseByteArrayElements(sharedSecret, (jbyte *)sharedTmp, 0);
    return (jboolean) result;
}
