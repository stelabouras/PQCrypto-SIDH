// To create the JNI interface file:
// - cd cpp/javaNative
// - javac -cp annotation-1.1.0.jar -d . sidhjava/test/SidhNativeTests.java
// - javah sidhjava.test.SidhNativeTests
//
// after this step you may remove the resulting class files in sidhjava/test sub-directory

package sidhjava.test;

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
