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

#include <cstddef>
#include <memory>

class SidhWrapper {
public:

    enum SidhType {
        P434 = 1,               //!< Normal P434 API
        P434Comp,               //!< Compressed 434
        P503,
        P503Comp,
        P610,
        P610Comp,
        P751,
        P751Comp
    };

    struct FieldLengths {
        size_t privateKeyA;     //!< length of private key field A
        size_t privateKeyB;     //!< length of private key field B
        size_t publicKey;       //!< length of public key field
        size_t sharedSecretA;   //!< length of shared secret A
        size_t sharedSecretB;   //!< length of shared secret B
    };
    using FieldLengthsPtr = std::unique_ptr<FieldLengths>;

    SidhWrapper() = delete;

    /**
     * @brief getLengths of SIDH fields.
     *
     * Returns the fields lengths required by the SIDH algorithms.
     *
     */

    /**
     * @brief Get lengths of SIDH fields based on SIDH type.
     *
     * @param[in] sidhType SIDH type
     * @return Unique pointer to @ FieldLengths structure which contains the field lengths
     */
    static FieldLengthsPtr getFieldLengths(SidhType sidhType);

    /** @brief Generation of Alice's secret key.
     *
     * Outputs random value in [0, 2^216 - 1] to be used as Alice's private key.
     *
     * @param[in] sidhType SIDH type
     * @param[out] randomDigits reference to an array of the length returned as first parameter
     *              by @c getLengths
     *
     * @sq getLengths(SidhType, size_t&, size_t& size_t&, size_t&, size_t&)
     */
    static void random_mod_order_A(SidhType sidhType, unsigned char *randomDigits);

    // Generation of Bob's secret key
    // Outputs random value in [0, 2^Floor(Log(2,3^137)) - 1] to be used as Bob's private key
    static void random_mod_order_B(SidhType sidhType, unsigned char *random_digits);

    // Alice's ephemeral public key generation
    // Input:  a private key PrivateKeyA in the range [0, 2^216 - 1], stored in 27 bytes.
    // Output: the public key PublicKeyA consisting of 3 GF(p434^2) elements encoded in 330 bytes.
    static bool EphemeralKeyGeneration_A(SidhType sidhType, const unsigned char *PrivateKeyA, unsigned char *PublicKeyA);

    // Bob's ephemeral key-pair generation
    // It produces a private key PrivateKeyB and computes the public key PublicKeyB.
    // The private key is an integer in the range [0, 2^Floor(Log(2,3^137)) - 1], stored in 28 bytes.
    // The public key consists of 3 GF(p434^2) elements encoded in 330 bytes.
    static bool EphemeralKeyGeneration_B(SidhType sidhType, const unsigned char *PrivateKeyB, unsigned char *PublicKeyB);

    // Alice's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
    // Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^216 - 1], stored in 27 bytes.
    //         Bob's PublicKeyB consists of 3 GF(p434^2) elements encoded in 330 bytes.
    // Output: a shared secret SharedSecretA that consists of one element in GF(p434^2) encoded in 110 bytes.
    static bool EphemeralSecretAgreement_A(SidhType sidhType, const unsigned char *PrivateKeyA, const unsigned char *PublicKeyB,
                                           unsigned char *SharedSecretA);

    // Bob's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
    // Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^137)) - 1], stored in 28 bytes.
    //         Alice's PublicKeyA consists of 3 GF(p434^2) elements encoded in 330 bytes.
    // Output: a shared secret SharedSecretB that consists of one element in GF(p434^2) encoded in 110 bytes.
    static bool EphemeralSecretAgreement_B(SidhType sidhType, const unsigned char *PrivateKeyB, const unsigned char *PublicKeyA,
                                           unsigned char *SharedSecretB);

private:

    static FieldLengthsPtr getFieldLengthsP434();
    static FieldLengthsPtr getFieldLengthsP503();
    static FieldLengthsPtr getFieldLengthsP610();
    static FieldLengthsPtr getFieldLengthsP751();

    // ************ SIDH 434 functions **************

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

    // ************ SIDH 503 functions **************

    // Generation of Alice's secret key
    // Outputs random value in [0, 2^250 - 1] to be used as Alice's private key
    static void random_mod_order_A_SIDHp503(unsigned char* random_digits);

    // Generation of Bob's secret key
    // Outputs random value in [0, 2^Floor(Log(2,3^159)) - 1] to be used as Bob's private key
    static void random_mod_order_B_SIDHp503(unsigned char* random_digits);

    // Alice's ephemeral public key generation
    // Input:  a private key PrivateKeyA in the range [0, 2^250 - 1], stored in 32 bytes.
    // Output: the public key PublicKeyA consisting of 3 GF(p503^2) elements encoded in 378 bytes.
    static int EphemeralKeyGeneration_A_SIDHp503(const unsigned char* PrivateKeyA, unsigned char* PublicKeyA);

    // Bob's ephemeral key-pair generation
    // It produces a private key PrivateKeyB and computes the public key PublicKeyB.
    // The private key is an integer in the range [0, 2^Floor(Log(2,3^159)) - 1], stored in 32 bytes.
    // The public key consists of 3 GF(p503^2) elements encoded in 378 bytes.
    static int EphemeralKeyGeneration_B_SIDHp503(const unsigned char* PrivateKeyB, unsigned char* PublicKeyB);

    // Alice's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
    // Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^250 - 1], stored in 32 bytes.
    //         Bob's PublicKeyB consists of 3 GF(p503^2) elements encoded in 378 bytes.
    // Output: a shared secret SharedSecretA that consists of one element in GF(p503^2) encoded in 126 bytes.
    static int EphemeralSecretAgreement_A_SIDHp503(const unsigned char* PrivateKeyA, const unsigned char* PublicKeyB, unsigned char* SharedSecretA);

    // Bob's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
    // Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^159)) - 1], stored in 32 bytes.
    //         Alice's PublicKeyA consists of 3 GF(p503^2) elements encoded in 378 bytes.
    // Output: a shared secret SharedSecretB that consists of one element in GF(p503^2) encoded in 126 bytes.
    static int EphemeralSecretAgreement_B_SIDHp503(const unsigned char* PrivateKeyB, const unsigned char* PublicKeyA, unsigned char* SharedSecretB);

    // ************ SIDH 610 functions **************

    // Generation of Alice's secret key
    // Outputs random value in [0, 2^305 - 1] to be used as Alice's private key
    static void random_mod_order_A_SIDHp610(unsigned char* random_digits);

    // Generation of Bob's secret key
    // Outputs random value in [0, 2^Floor(Log(2,3^192)) - 1] to be used as Bob's private key
    static void random_mod_order_B_SIDHp610(unsigned char* random_digits);

    // Alice's ephemeral public key generation
    // Input:  a private key PrivateKeyA in the range [0, 2^305 - 1], stored in 38 bytes.
    // Output: the public key PublicKeyA consisting of 3 GF(p610^2) elements encoded in 462 bytes.
    static int EphemeralKeyGeneration_A_SIDHp610(const unsigned char* PrivateKeyA, unsigned char* PublicKeyA);

    // Bob's ephemeral key-pair generation
    // It produces a private key PrivateKeyB and computes the public key PublicKeyB.
    // The private key is an integer in the range [0, 2^Floor(Log(2,3^192)) - 1], stored in 38 bytes.
    // The public key consists of 3 GF(p610^2) elements encoded in 462 bytes.
    static int EphemeralKeyGeneration_B_SIDHp610(const unsigned char* PrivateKeyB, unsigned char* PublicKeyB);

    // Alice's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
    // Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^305 - 1], stored in 38 bytes.
    //         Bob's PublicKeyB consists of 3 GF(p610^2) elements encoded in 462 bytes.
    // Output: a shared secret SharedSecretA that consists of one element in GF(p610^2) encoded in 154 bytes.
    static int EphemeralSecretAgreement_A_SIDHp610(const unsigned char* PrivateKeyA, const unsigned char* PublicKeyB, unsigned char* SharedSecretA);

    // Bob's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
    // Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^192)) - 1], stored in 38 bytes.
    //         Alice's PublicKeyA consists of 3 GF(p610^2) elements encoded in 462 bytes.
    // Output: a shared secret SharedSecretB that consists of one element in GF(p610^2) encoded in 154 bytes.
    static int EphemeralSecretAgreement_B_SIDHp610(const unsigned char* PrivateKeyB, const unsigned char* PublicKeyA, unsigned char* SharedSecretB);

    // ************ SIDH 751 functions **************

    // Generation of Alice's secret key
    // Outputs random value in [0, 2^372 - 1] to be used as Alice's private key
    static void random_mod_order_A_SIDHp751(unsigned char* random_digits);

    // Generation of Bob's secret key
    // Outputs random value in [0, 2^Floor(Log(2,3^239)) - 1] to be used as Bob's private key
    static void random_mod_order_B_SIDHp751(unsigned char* random_digits);

    // Alice's ephemeral public key generation
    // Input:  a private key PrivateKeyA in the range [0, 2^372 - 1], stored in 47 bytes.
    // Output: the public key PublicKeyA consisting of 3 GF(p751^2) elements encoded in 564 bytes.
    static int EphemeralKeyGeneration_A_SIDHp751(const unsigned char* PrivateKeyA, unsigned char* PublicKeyA);

    // Bob's ephemeral key-pair generation
    // It produces a private key PrivateKeyB and computes the public key PublicKeyB.
    // The private key is an integer in the range [0, 2^Floor(Log(2,3^239)) - 1], stored in 48 bytes.
    // The public key consists of 3 GF(p751^2) elements encoded in 564 bytes.
    static int EphemeralKeyGeneration_B_SIDHp751(const unsigned char* PrivateKeyB, unsigned char* PublicKeyB);

    // Alice's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
    // Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^372 - 1], stored in 47 bytes.
    //         Bob's PublicKeyB consists of 3 GF(p751^2) elements encoded in 564 bytes.
    // Output: a shared secret SharedSecretA that consists of one element in GF(p751^2) encoded in 188 bytes.
    static int EphemeralSecretAgreement_A_SIDHp751(const unsigned char* PrivateKeyA, const unsigned char* PublicKeyB, unsigned char* SharedSecretA);

    // Bob's ephemeral shared secret computation
    // It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
    // Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^239)) - 1], stored in 48 bytes.
    //         Alice's PublicKeyA consists of 3 GF(p751^2) elements encoded in 564 bytes.
    // Output: a shared secret SharedSecretB that consists of one element in GF(p751^2) encoded in 188 bytes.
    static int EphemeralSecretAgreement_B_SIDHp751(const unsigned char* PrivateKeyB, const unsigned char* PublicKeyA, unsigned char* SharedSecretB);

};


#endif //SIDH_SIDHWRAPPER_H
