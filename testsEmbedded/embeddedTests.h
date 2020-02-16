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
// Created by werner on 14.02.20.
// Copyright (c) 2020 Werner Dittmann. All rights reserved.
//

#ifndef PQCRYPTO_SIDH_EMBEDDEDTESTS_H
#define PQCRYPTO_SIDH_EMBEDDEDTESTS_H

#ifdef __ANDROID__
#define PRINTF Log
#ifdef __cplusplus
extern "C" {
#endif
void Log(char const *format, ...);
#ifdef __cplusplus
}
#endif

#else
#define PRINTF printf
#endif

#undef print_unit
#if (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
#define print_unit PRINTF("nsec");
#else
#define print_unit printf("cycles");
#endif

/**
 * @brief Bitmask to define which test functions to run.
 */
enum RunTestBits {
    TestsOnly = 1,          //!< Test the algorithms only - fast
    TestBenchmarks = 2,     //!< Benchmarks the algorithms - moderate
    EcisogFunctions = 4     //!< Benchmark EC Isogenic function - takes some time :-)
};

#endif //PQCRYPTO_SIDH_EMBEDDEDTESTS_H
