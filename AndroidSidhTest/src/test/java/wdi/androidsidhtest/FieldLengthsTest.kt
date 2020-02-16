package wdi.androidsidhtest

import org.junit.Assert.assertEquals
import org.junit.Assert.assertNotNull
import org.junit.BeforeClass
import org.junit.Test
import sidh.SidhNative
import java.io.File

/**
 * SIDH Kotlin unit tests, which execute on the development machine (host).
 *
 * NOTE: check library load path to make sure the loader finds the correct library.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class FieldLengthsTest {

    companion object {
        @BeforeClass
        @JvmStatic
        fun loadLibs() {
            val currentDir = File(".")
            // print("Dir: ${currentDir.canonicalPath}")   // Just to see the path and fix below if necessary
            System.load("${currentDir.canonicalPath}/../cmake-build-debug/libsidhCppLib.so")
        }

        // The values below copied from P{434, 503, 610, 751}_api.h files
        const val SIDH_434_SECRETKEYBYTES_A =   27L
        const val SIDH_434_SECRETKEYBYTES_B =   28L
        const val SIDH_434_PUBLICKEYBYTES   =  330L
        const val SIDH_434_BYTES            =  110L

        const val SIDH_503_SECRETKEYBYTES_A =   32L
        const val SIDH_503_SECRETKEYBYTES_B =   32L
        const val SIDH_503_PUBLICKEYBYTES   =  378L
        const val SIDH_503_BYTES            =  126L

        const val SIDH_610_SECRETKEYBYTES_A =   39L
        const val SIDH_610_SECRETKEYBYTES_B =   38L
        const val SIDH_610_PUBLICKEYBYTES   =  462L
        const val SIDH_610_BYTES            =  154L

        const val SIDH_751_SECRETKEYBYTES_A =   47L
        const val SIDH_751_SECRETKEYBYTES_B =   48L
        const val SIDH_751_PUBLICKEYBYTES   =  564L
        const val SIDH_751_BYTES            =  188L
    }

    @Test
    fun checkFieldLengths434() {
        val lengths = SidhNative.getFieldLengths(SidhNative.P434)
        assertNotNull(lengths)
        lengths?.let {
            assertEquals(SIDH_434_SECRETKEYBYTES_A, it.privateKeyA)
            assertEquals(SIDH_434_SECRETKEYBYTES_B, it.privateKeyB)
            assertEquals(SIDH_434_PUBLICKEYBYTES, it.publicKey)
            assertEquals(SIDH_434_BYTES, it.sharedSecret)
        }
    }

    @Test
    fun checkFieldLengths503() {
        val lengths = SidhNative.getFieldLengths(SidhNative.P503)
        assertNotNull(lengths)
        lengths?.let {
            assertEquals(SIDH_503_SECRETKEYBYTES_A, it.privateKeyA)
            assertEquals(SIDH_503_SECRETKEYBYTES_B, it.privateKeyB)
            assertEquals(SIDH_503_PUBLICKEYBYTES, it.publicKey)
            assertEquals(SIDH_503_BYTES, it.sharedSecret)
        }
    }

    @Test
    fun checkFieldLengths610() {
        val lengths = SidhNative.getFieldLengths(SidhNative.P610)
        assertNotNull(lengths)
        lengths?.let {
            assertEquals(SIDH_610_SECRETKEYBYTES_A, it.privateKeyA)
            assertEquals(SIDH_610_SECRETKEYBYTES_B, it.privateKeyB)
            assertEquals(SIDH_610_PUBLICKEYBYTES, it.publicKey)
            assertEquals(SIDH_610_BYTES, it.sharedSecret)
        }
    }

    @Test
    fun checkFieldLengths751() {
        val lengths = SidhNative.getFieldLengths(SidhNative.P751)
        assertNotNull(lengths)
        lengths?.let {
            assertEquals(SIDH_751_SECRETKEYBYTES_A, it.privateKeyA)
            assertEquals(SIDH_751_SECRETKEYBYTES_B, it.privateKeyB)
            assertEquals(SIDH_751_PUBLICKEYBYTES, it.publicKey)
            assertEquals(SIDH_751_BYTES, it.sharedSecret)
        }
    }
}
