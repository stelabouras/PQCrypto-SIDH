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
// Created by werner on 09.02.20.
// Copyright (c) 2020 Werner Dittmann. All rights reserved.
//

#ifndef SIDH_COMMON_FUNCS_H
#define SIDH_COMMON_FUNCS_H

#include "../config.h"

// Multiprecision left shift
void mp_shiftleft(digit_t* x, unsigned int shift, const unsigned int nwords);

// Multiprecision right shift by one
void mp_shiftr1(digit_t* x, const unsigned int nwords);

// Multiprecision left right shift by one
void mp_shiftl1(digit_t* x, const unsigned int nwords);

void clear_words(void* mem, digit_t nwords);

// Copy wordsize digits, c = a, where lng(a) = nwords
void copy_words(const digit_t* a, digit_t* c, const unsigned int nwords);

#endif //SIDH_COMMON_FUNCS_H
