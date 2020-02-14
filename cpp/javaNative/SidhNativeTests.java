// To create the JNI interface file:
// - cd to the SidhTestNative.java directory
// - run 'javac -d . SidhNativeTests.java'
// - run 'javah sidh.tests.SidhNativeTests'
//
// After this you can remove the created 'sidh.tests' directory.

package sidh.tests;

public abstract class SidhNativeTests {

    /**
     * Bitmask to define which test functions to run.
     */
    public static final int TestsOnly = 1;          //!< Test the algorithms only - fast
    public static final int TestBenchmarks = 2;     //!< Benchmarks the algorithms - moderate
    public static final int EcisogFunctions = 4;    //!< Benchmark EC Isogenic function - takes some time :-)
    public static final int DiffieHellman = 8;      //!< Diffie-Hellman key exchange tests

    public static native boolean runSidhTests(int sidhType, int testMask);
}
