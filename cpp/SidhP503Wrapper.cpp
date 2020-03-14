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
// Created by werner on 07.02.20.
// Copyright (c) 2020 Werner Dittmann. All rights reserved.
//
#include "SidhWrapper.h"

// Include the P503 headers as plain C to get correct functions names
#if defined(__cplusplus)
extern "C"
{
#endif
#include "P503/P503_api.h"
#if defined(__cplusplus)
}
#endif

SidhWrapper::FieldLengthsPtr SidhWrapper::getFieldLengthsP503() {
    auto fieldLengths = std::make_unique<SidhWrapper::FieldLengths>();

    fieldLengths->publicKey = SIDH_PUBLICKEYBYTES;
    fieldLengths->privateKeyA = SIDH_SECRETKEYBYTES_A;
    fieldLengths->privateKeyB = SIDH_SECRETKEYBYTES_B;
    fieldLengths->sharedSecret = SIDH_BYTES;
    return fieldLengths;
}

void SidhWrapper::random_mod_order_A_SIDHp503(unsigned char *random_digits) {
    ::random_mod_order_A_SIDHp503(random_digits);
}
void SidhWrapper::random_mod_order_B_SIDHp503(unsigned char *random_digits) {
    ::random_mod_order_B_SIDHp503(random_digits);
}
int SidhWrapper::EphemeralKeyGeneration_A_SIDHp503(const unsigned char *PrivateKeyA, unsigned char *PublicKeyA) {
    return ::EphemeralKeyGeneration_A_SIDHp503(PrivateKeyA, PublicKeyA);
}
int SidhWrapper::EphemeralKeyGeneration_B_SIDHp503(const unsigned char *PrivateKeyB, unsigned char *PublicKeyB) {
    return ::EphemeralKeyGeneration_B_SIDHp503(PrivateKeyB, PublicKeyB);
}
int SidhWrapper::EphemeralSecretAgreement_A_SIDHp503(const unsigned char *PrivateKeyA, const unsigned char *PublicKeyB,
                                                     unsigned char *SharedSecretA) {
    return ::EphemeralSecretAgreement_A_SIDHp503(PrivateKeyA, PublicKeyB, SharedSecretA);
}
int SidhWrapper::EphemeralSecretAgreement_B_SIDHp503(const unsigned char *PrivateKeyB, const unsigned char *PublicKeyA,
                                                     unsigned char *SharedSecretB) {
    return ::EphemeralSecretAgreement_B_SIDHp503(PrivateKeyB, PublicKeyA, SharedSecretB);
}
