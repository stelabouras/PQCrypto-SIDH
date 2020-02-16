// To create the JNI interface file:
// - cd cpp/javaNative
// - javac -cp annotation-1.1.0.jar -d . sidhjava/SidhNative.java
// - javah sidhjava.SidhNative
//
// after this step you may remove the resulting class files in sidhjava sub-directory

package sidhjava;

import androidx.annotation.Nullable;
import androidx.annotation.NonNull;

public abstract class SidhNative {

    /**
     * Which SIDH type/length to use.
     */
    public static final int P434 = 1;               //!< Normal P434 API
    public static final int P434Comp = 2;           //!< Compressed 434
    public static final int P503 = 3;
    public static final int P503Comp = 4;
    public static final int P610 = 5;
    public static final int P610Comp = 6;
    public static final int P751 = 7;
    public static final int P751Comp = 8;

    /**
     * Class returned by native getFieldLengths function.
     */
    public static class FieldLengths {
        public long privateKeyA;     //!< length of private key field Alice
        public long privateKeyB;     //!< length of private key field Bob
        public long publicKey;       //!< length of public key field
        public long sharedSecret;    //!< length of shared secret

        public FieldLengths() {}
    }

    /**
     * Enable / set logging callback in native library.
     *
     * @see public abstract void loggingCallback(int level, byte[] data);
     */
    public native int enableLoggingCallback();

    /**
     * Disable logging callback in native library.
     *
     * @see public abstract void loggingCallback(int level, byte[] data);
     */
    public static native void disableLoggingCallback();

    /**
     * Get lengths of SIDH fields based on SIDH type.
     *
     * @param sidhType SIDH type/algorithm
     * @return {@code FieldLengths} class which contains the field lengths
     */
    @Nullable
    public static native FieldLengths getFieldLengths(int sidhType);

    /** Generation of Alice's secret key.
     *
     * Outputs random suitable for the selected SIDH type value to be used as Alice's private key.
     *
     * @param sidhType SIDH type/algorithm
     * @param randomDigits reference to an array of the length returned as first parameter
     *              in  {@code FieldLengths}
     *
     * @see FieldLengths getFieldLengths(int)
     */
    public static native boolean randomModOrderA(int sidhType, @NonNull byte[] randomDigits);

    /** Generation of Alice's secret key.
     *
     * Outputs random value suitable for the selected SIDH type to be used as Bob's private key
     *
     * @param sidhType SIDH type/algorithm
     * @param randomDigits reference to an array of the length returned as second parameter
     *              in  {@code FieldLengths}
     *
     * @see FieldLengths getFieldLengths(int)
     */
    public static native boolean randomModOrderB(int sidhType, @NonNull byte[] randomDigits);

    /**
     * Alice's ephemeral public key generation.
     *
     * @param sidhType SIDH type/algorithm
     * @param PrivateKeyA Input: the private key, length as returned in first parameter in {@code FieldLengths}
     * @param PublicKeyA Output: the public key, length as returned in third parameter in {@code FieldLengths}
     * @return {@code true} if key generation was OK
     */
    public static native boolean ephemeralKeyGenerationA(int sidhType, @NonNull byte[] PrivateKeyA, @NonNull byte[] PublicKeyA);

    /**
     * Bob's ephemeral public key generation.
     *
     * @param sidhType SIDH type/algorithm
     * @param PrivateKeyB Input: the private key, length as returned in second parameter in {@code FieldLengths}
     * @param PublicKeyB Output: the public key, length as returned in third parameter in {@code FieldLengths}
     * @return {@code true} if key generation was OK
     */
    public static native boolean ephemeralKeyGenerationB(int sidhType, @NonNull byte[] PrivateKeyB, @NonNull byte[] PublicKeyB);

    /**
     * Alice's ephemeral shared secret computation.
     *
     * It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
     *
     * @param sidhType SIDH type/algorithm
     * @param PrivateKeyA Input: Alice's PrivateKeyA, length as returned in first parameter in {@code FieldLengths}
     * @param PublicKeyB Input: Bob's PublicKeyB, length as returned in third parameter in {@code FieldLengths}
     * @param SharedSecretA Output: a shared secret SharedSecretA, length as returned in fourth parameter in {@code FieldLengths}
     * @return {@code true} if key agreement was OK
     */
    public static native boolean ephemeralSecretAgreementA(int sidhType, @NonNull byte[] PrivateKeyA, @NonNull byte[] PublicKeyB,
                                                            @NonNull byte[] SharedSecretA);

    /**
     * Bob's ephemeral shared secret computation.
     *
     * It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
     *
     * @param sidhType Bob's ephemeral shared secret computation
     * @param PrivateKeyB Input: Bob's PrivateKeyB, length as returned in second parameter in {@code FieldLengths}
     * @param PublicKeyA Input: Alice's PublicKeyA, length as returned in third parameter in {@code FieldLengths}
     * @param SharedSecretB Output: a shared secret SharedSecretB, length as returned in fourth parameter in {@code FieldLengths}
     * @return {@code true} if key agreement was OK
     */
    public static native boolean ephemeralSecretAgreementB(int sidhType, @NonNull byte[] PrivateKeyB, @NonNull byte[] PublicKeyA,
                                                            @NonNull byte[] SharedSecretB);

    /**
     * Logging callback.
     *
     * The library uses this callback to forward logging data to the app.
     *
     * @param level Log level, 0: No log, 1: Error, 2: Warning, 3: Info, 4: Debug, 5: Verbose, 6: Epic
     * @param data  Logging data as String
     */
    public abstract void loggingCallback(int level, String data);
}
