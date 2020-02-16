// To create the JNI interface file:
// - cd cpp/javaNative
// - javac -cp annotation-1.1.0.jar -d . sidhjava/SidhNative.java
// - javah sidhjava.SidhNative
//
// after this step you may remove the resulting class files in sidhjava sub-directory

package sidhjava;

import androidx.annotation.Nullable;

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
     * Get the field lengths of the selected SIDH algorithm
     */
    @Nullable
    public static native FieldLengths getFieldLengths(int sidhType);

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
