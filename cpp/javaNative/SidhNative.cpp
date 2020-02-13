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

#include "sidh_tests_SidhNativeTests.h"

/**
 * Define -DPACKAGE_NAME="Java_some_package_name_" to define another package
 * name during compilation
 */
#ifndef PACKAGE_NAME_TESTS
#define PACKAGE_NAME_TESTS Java_sidh_tests_SidhNativeTests_
#endif

#ifndef PACKAGE_NAME
#define PACKAGE_NAME Java_sidh_SidhNative_
#endif

#define CONCATx(a,b) a##b
#define CONCAT(a,b) CONCATx(a,b)

#define JNI_FUNCTION_TESTS(FUNC_NAME)  CONCAT(PACKAGE_NAME_TESTS, FUNC_NAME)

#ifndef JNIEXPORT
#error "JNIEXPORT not defined"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ANDROID__
void Log(char const *format, ...) {
    va_list arg;
    va_start(arg, format);

    __android_log_vprint(ANDROID_LOG_DEBUG, "libsidh", format, arg);
    va_end(arg);
}
#endif

#ifdef ADD_TESTS
int arithTests434();
#endif

#ifdef __cplusplus
}
#endif

#ifdef ADD_TESTS
/*
 * Class:     sidh_SidhTestNative
 * Method:    runSidhTest
 * Signature: ()I
 */
JNIEXPORT jint
JNI_FUNCTION_TESTS(runSidhTest)(JNIEnv *env, jclass clazz)
{
    (void)env;
    (void)clazz;

    return arithTests434();
}
#endif
