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

#include "SidhWrapper.h"

SidhWrapper::FieldLengthsPtr SidhWrapper::getFieldLengths(SidhType sidhType) {
    switch (sidhType) {
        case P434:
            return getFieldLengthsP434();
        case P434Comp:
            break;

        case P503:
            return getFieldLengthsP503();
        case P503Comp:
            break;

        case P610:
            return getFieldLengthsP610();
        case P610Comp:
            break;

        case P751:
            return getFieldLengthsP751();
        case P751Comp:
            break;

    }
    return std::unique_ptr<FieldLengths>();
}

void SidhWrapper::random_mod_order_A(SidhType sidhType, unsigned char *randomDigits){
    switch (sidhType) {
        case P434:
            random_mod_order_A_SIDHp434(randomDigits);
            break;
        case P434Comp:
            break;

        case P503:
            random_mod_order_A_SIDHp503(randomDigits);
            break;
        case P503Comp:
            break;

        case P610:
            random_mod_order_A_SIDHp610(randomDigits);
            break;
        case P610Comp:
            break;

        case P751:
            random_mod_order_A_SIDHp751(randomDigits);
            break;
        case P751Comp:
            break;

    }
}

void SidhWrapper::random_mod_order_B(SidhType sidhType, unsigned char *random_digits){
    switch (sidhType) {
        case P434:
            return random_mod_order_B_SIDHp434(random_digits);
        case P434Comp:
            break;

        case P503:
            return random_mod_order_B_SIDHp503(random_digits);
        case P503Comp:
            break;

        case P610:
            return random_mod_order_B_SIDHp610(random_digits);
        case P610Comp:
            break;

        case P751:
            return random_mod_order_B_SIDHp751(random_digits);
        case P751Comp:
            break;

    }
}

bool SidhWrapper::EphemeralKeyGeneration_A(SidhType sidhType, const unsigned char *PrivateKeyA, unsigned char *PublicKeyA){
    switch (sidhType) {
        case P434:
            return EphemeralKeyGeneration_A_SIDHp434(PrivateKeyA, PublicKeyA) == 0;
        case P434Comp:
            break;

        case P503:
            return EphemeralKeyGeneration_A_SIDHp503(PrivateKeyA, PublicKeyA) == 0;
        case P503Comp:
            break;

        case P610:
            return EphemeralKeyGeneration_A_SIDHp610(PrivateKeyA, PublicKeyA) == 0;
        case P610Comp:
            break;

        case P751:
            return EphemeralKeyGeneration_A_SIDHp751(PrivateKeyA, PublicKeyA) == 0;
        case P751Comp:
            break;

    }
    return false;
}

bool SidhWrapper::EphemeralKeyGeneration_B(SidhType sidhType, const unsigned char *PrivateKeyB, unsigned char *PublicKeyB){
    switch (sidhType) {
        case P434:
            return EphemeralKeyGeneration_B_SIDHp434(PrivateKeyB, PublicKeyB) == 0;
        case P434Comp:
            break;

        case P503:
            return EphemeralKeyGeneration_B_SIDHp503(PrivateKeyB, PublicKeyB) == 0;
        case P503Comp:
            break;

        case P610:
            return EphemeralKeyGeneration_B_SIDHp610(PrivateKeyB, PublicKeyB) == 0;
        case P610Comp:
            break;

        case P751:
            return EphemeralKeyGeneration_B_SIDHp751(PrivateKeyB, PublicKeyB) == 0;
        case P751Comp:
            break;

    }
    return false;
}

bool SidhWrapper::EphemeralSecretAgreement_A(SidhType sidhType, const unsigned char *PrivateKeyA, const unsigned char *PublicKeyB,
                                       unsigned char *SharedSecretA){
    switch (sidhType) {
        case P434:
            return EphemeralSecretAgreement_A_SIDHp434(PrivateKeyA, PublicKeyB, SharedSecretA) == 0;
        case P434Comp:
            break;

        case P503:
            return EphemeralSecretAgreement_A_SIDHp503(PrivateKeyA, PublicKeyB, SharedSecretA) == 0;
        case P503Comp:
            break;

        case P610:
            return EphemeralSecretAgreement_A_SIDHp610(PrivateKeyA, PublicKeyB, SharedSecretA) == 0;
        case P610Comp:
            break;

        case P751:
            return EphemeralSecretAgreement_A_SIDHp751(PrivateKeyA, PublicKeyB, SharedSecretA) == 0;
        case P751Comp:
            break;

    }
    return false;
}

bool SidhWrapper::EphemeralSecretAgreement_B(SidhType sidhType, const unsigned char *PrivateKeyB, const unsigned char *PublicKeyA,
                                       unsigned char *SharedSecretB){
    switch (sidhType) {
        case P434:
            return EphemeralSecretAgreement_B_SIDHp434(PrivateKeyB, PublicKeyA, SharedSecretB) == 0;
        case P434Comp:
            break;

        case P503:
            return EphemeralSecretAgreement_B_SIDHp503(PrivateKeyB, PublicKeyA, SharedSecretB) == 0;
        case P503Comp:
            break;

        case P610:
            return EphemeralSecretAgreement_B_SIDHp610(PrivateKeyB, PublicKeyA, SharedSecretB) == 0;
        case P610Comp:
            break;

        case P751:
            return EphemeralSecretAgreement_B_SIDHp751(PrivateKeyB, PublicKeyA, SharedSecretB) == 0;
        case P751Comp:
            break;

    }
    return false;
}

