// To create the JNI interface file:
// - cd to the SidhTestNative.java directory
// - run 'javac -d . SidhNativeTests.java'
// - run 'javah sidh.tests.SidhNativeTests'
//
// After this you can remove the created 'sidh.tests' directory.

package sidh.tests;

public abstract class SidhNativeTests {

    public static native int runSidhTests();

}
