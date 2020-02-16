package wdi.androidsidhtest

import org.junit.Assert.*
import org.junit.BeforeClass
import org.junit.Test
import sidhjava.SidhNative.*
import java.io.File

/**
 * SIDH Kotlin unit tests, which execute on the development machine (host).
 *
 * NOTE: check library load path to make sure the loader finds the correct library.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class DiffieHellmanTest {

    companion object {
        @BeforeClass
        @JvmStatic
        fun loadLibs() {
            val currentDir = File(".")
            // print("Dir: ${currentDir.canonicalPath}")   // Just to see the path and fix below if necessary
            System.load("${currentDir.canonicalPath}/../cmake-build-debug/libsidhCppLib.so")
        }
    }

    @Test
    fun getRandomForPrivate434() {
        val sidhType = P434
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())
            val wrongArray = ByteArray(1)

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            // println just to insert a newline between log output lines
            assertFalse(randomModOrderA(sidhType, wrongArray))
            println()
            assertFalse(randomModOrderB(sidhType, wrongArray))
            println()
        }
    }

    @Test
    fun getRandomForPrivate503() {
        val sidhType = P503
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())

            val wrongArray = ByteArray(1)

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            // println just to insert a newline between log output lines
            assertFalse(randomModOrderA(sidhType, wrongArray))
            println()
            assertFalse(randomModOrderB(sidhType, wrongArray))
            println()
        }
    }

    @Test
    fun getRandomForPrivate610() {
        val sidhType = P610
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())
            val wrongArray = ByteArray(1)

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            // println just to insert a newline between log output lines
            assertFalse(randomModOrderA(sidhType, wrongArray))
            println()
            assertFalse(randomModOrderB(sidhType, wrongArray))
            println()
        }
    }

    @Test
    fun getRandomForPrivate751() {
        val sidhType = P751
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())
            val wrongArray = ByteArray(1)

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            // println just to insert a newline between log output lines
            assertFalse(randomModOrderA(sidhType, wrongArray))
            println()
            assertFalse(randomModOrderB(sidhType, wrongArray))
            println()
        }
    }

    @Test
    fun keyAgreement434() {
        val sidhType = P434
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())

            val publicA = ByteArray(lengths.publicKey.toInt())
            val publicB = ByteArray(lengths.publicKey.toInt())

            val secretA = ByteArray(lengths.sharedSecret.toInt())
            val secretB = ByteArray(lengths.sharedSecret.toInt())

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            assertTrue(ephemeralKeyGenerationA(sidhType, privateA, publicA))
            assertTrue(ephemeralKeyGenerationB(sidhType, privateB, publicB))

            assertTrue(ephemeralSecretAgreementA(sidhType, privateA, publicB, secretA))
            assertTrue(ephemeralSecretAgreementB(sidhType, privateB, publicA, secretB))

            assertTrue(secretA.contentEquals(secretB))
        }
    }

    @Test
    fun keyAgreement503() {
        val sidhType = P503
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())

            val publicA = ByteArray(lengths.publicKey.toInt())
            val publicB = ByteArray(lengths.publicKey.toInt())

            val secretA = ByteArray(lengths.sharedSecret.toInt())
            val secretB = ByteArray(lengths.sharedSecret.toInt())

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            assertTrue(ephemeralKeyGenerationA(sidhType, privateA, publicA))
            assertTrue(ephemeralKeyGenerationB(sidhType, privateB, publicB))

            assertTrue(ephemeralSecretAgreementA(sidhType, privateA, publicB, secretA))
            assertTrue(ephemeralSecretAgreementB(sidhType, privateB, publicA, secretB))

            assertTrue(secretA.contentEquals(secretB))
        }
    }

    @Test
    fun keyAgreement610() {
        val sidhType = P610
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())

            val publicA = ByteArray(lengths.publicKey.toInt())
            val publicB = ByteArray(lengths.publicKey.toInt())

            val secretA = ByteArray(lengths.sharedSecret.toInt())
            val secretB = ByteArray(lengths.sharedSecret.toInt())

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            assertTrue(ephemeralKeyGenerationA(sidhType, privateA, publicA))
            assertTrue(ephemeralKeyGenerationB(sidhType, privateB, publicB))

            assertTrue(ephemeralSecretAgreementA(sidhType, privateA, publicB, secretA))
            assertTrue(ephemeralSecretAgreementB(sidhType, privateB, publicA, secretB))

            assertTrue(secretA.contentEquals(secretB))
        }
    }

    @Test
    fun keyAgreement751() {
        val sidhType = P751
        val lengths = getFieldLengths(sidhType)
        assertNotNull(lengths)

        lengths?.let {
            val privateA = ByteArray(lengths.privateKeyA.toInt())
            val privateB = ByteArray(lengths.privateKeyB.toInt())

            val publicA = ByteArray(lengths.publicKey.toInt())
            val publicB = ByteArray(lengths.publicKey.toInt())

            val secretA = ByteArray(lengths.sharedSecret.toInt())
            val secretB = ByteArray(lengths.sharedSecret.toInt())

            assertTrue(randomModOrderA(sidhType, privateA))
            assertTrue(randomModOrderB(sidhType, privateB))

            assertTrue(ephemeralKeyGenerationA(sidhType, privateA, publicA))
            assertTrue(ephemeralKeyGenerationB(sidhType, privateB, publicB))

            assertTrue(ephemeralSecretAgreementA(sidhType, privateA, publicB, secretA))
            assertTrue(ephemeralSecretAgreementB(sidhType, privateB, publicA, secretB))

            assertTrue(secretA.contentEquals(secretB))
        }
    }
}
