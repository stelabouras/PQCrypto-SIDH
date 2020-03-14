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
// Created by werner on 08.02.20.
// Copyright (c) 2020 Werner Dittmann. All rights reserved.
//

#include "../cpp/SidhWrapper.h"
#include "../src/P610/P610_api.h"
#include "gtest/gtest.h"

using namespace std;

class SidhTests610: public ::testing::Test {
public:
    SidhTests610() = default;

    SidhTests610(const SidhTests610& other) = delete;
    SidhTests610(const SidhTests610&& other) = delete;
    SidhTests610& operator= (const SidhTests610& other) = delete;
    SidhTests610& operator= (const SidhTests610&& other) = delete;

    void SetUp() override {
        // code here will execute just before the test ensues
    }

    void TearDown( ) override {
        // code here will be called just after the test completes
        // ok to through exceptions from here if need be
    }

    ~SidhTests610( ) override {
        // cleanup any pending stuff, but no exceptions allowed
    }
};

TEST_F(SidhTests610, FieldLengths) {
    auto lengths = SidhWrapper::getFieldLengths(SidhWrapper::P610);
    ASSERT_EQ(SIDH_SECRETKEYBYTES_A, lengths->privateKeyA);
    ASSERT_EQ(SIDH_SECRETKEYBYTES_B, lengths->privateKeyB);
    ASSERT_EQ(SIDH_PUBLICKEYBYTES, lengths->publicKey);
    ASSERT_EQ(SIDH_BYTES, lengths->sharedSecret);
}

TEST_F(SidhTests610, EphemeralSecret) {
    auto lengths = SidhWrapper::getFieldLengths(SidhWrapper::P610);
    auto privateKeyA = std::make_unique<uint8_t []>(lengths->privateKeyA);
    auto privateKeyB = std::make_unique<uint8_t []>(lengths->privateKeyB);
    auto publicKeyA = std::make_unique<uint8_t []>(lengths->publicKey);
    auto publicKeyB = std::make_unique<uint8_t []>(lengths->publicKey);

    auto sharedA = std::make_unique<uint8_t []>(lengths->sharedSecret);
    auto sharedB = std::make_unique<uint8_t []>(lengths->sharedSecret);

    SidhWrapper::random_mod_order_A(SidhWrapper::P610, privateKeyA.get());
    SidhWrapper::random_mod_order_B(SidhWrapper::P610, privateKeyB.get());

    SidhWrapper::EphemeralKeyGeneration_A(SidhWrapper::P610, privateKeyA.get(), publicKeyA.get());
    SidhWrapper::EphemeralKeyGeneration_B(SidhWrapper::P610, privateKeyB.get(), publicKeyB.get());

    // Alice computes her shared secret using Bob's public key
    SidhWrapper::EphemeralSecretAgreement_A(SidhWrapper::P610, privateKeyA.get(), publicKeyB.get(), sharedA.get());

    // Bob computes his shared secret using Alice's public key
    SidhWrapper::EphemeralSecretAgreement_B(SidhWrapper::P610, privateKeyB.get(), publicKeyA.get(), sharedB.get());

    ASSERT_TRUE(memcmp(sharedA.get(), sharedB.get(), lengths->sharedSecret) == 0);
}
