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

#ifndef SIDH_SIDHWRAPPER_H
#define SIDH_SIDHWRAPPER_H

class SidhWrapper {
public:
    SidhWrapper() = delete;

    // Generation of Alice's secret key
    // Outputs random value in [0, 2^216 - 1] to be used as Alice's private key
    static void random_mod_order_A_SIDHp434(unsigned char *random_digits);

    // Generation of Bob's secret key
    // Outputs random value in [0, 2^Floor(Log(2,3^137)) - 1] to be used as Bob's private key
    static void random_mod_order_B_SIDHp434(unsigned char *random_digits);

    // Alice's ephemeral public key generation
    // Input:  a private key PrivateKeyA in the range [0, 2^216 - 1], stored in 27 bytes.
    // Output: the public key PublicKeyA consisting of 3 GF(p434^2) elements encoded in 330 bytes.
    static int EphemeralKeyGeneration_A_SIDHp434(const unsigned char *PrivateKeyA, unsigned char *PublicKeyA);

    // Bob's ephemeral key-pair generation
    // It produces a private key PrivateKeyB and computes the public key PublicKeyB.
    // The private key is an integer in the range [0, 2^Floor(Log(2,3^137)) - 1], stored in 28 bytes.
    // The public key consists of 3 GF(p434^2) elements encoded in 330 bytes.
    static int EphemeralKeyGeneration_B_SIDHp434(const unsigned char *PrivateKeyB, unsigned char *PublicKeyB);

    // Alice's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
    // Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^216 - 1], stored in 27 bytes.
    //         Bob's PublicKeyB consists of 3 GF(p434^2) elements encoded in 330 bytes.
    // Output: a shared secret SharedSecretA that consists of one element in GF(p434^2) encoded in 110 bytes.
    static int EphemeralSecretAgreement_A_SIDHp434(const unsigned char *PrivateKeyA, const unsigned char *PublicKeyB,
                                                   unsigned char *SharedSecretA);

    // Bob's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
    // Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^137)) - 1], stored in 28 bytes.
    //         Alice's PublicKeyA consists of 3 GF(p434^2) elements encoded in 330 bytes.
    // Output: a shared secret SharedSecretB that consists of one element in GF(p434^2) encoded in 110 bytes.
    static int EphemeralSecretAgreement_B_SIDHp434(const unsigned char *PrivateKeyB, const unsigned char *PublicKeyA,
                                                   unsigned char *SharedSecretB);

};


#endif //SIDH_SIDHWRAPPER_H
