# SIDH v3.4 C++ wrapper, shared library, Android, and Java/Kotlin support 

The original content of SIDH README is below.

## Foreword and disclaimer :-)
Back in 2017 Phil Zimmerman pointed me to the SIDH algorithm and I added SIDH to my 
implementation of ZRTPCPP (available on github) as an enhancement to the normal 
Diffie-Hellman key algorithms. Because SIDH was in a very early stage at that point
I just did as an experimental feature and used the *SilentPhone* Android client to
set-up ZRTP with SIDH and had some first VoIP calls using ZRTP/SIDH to negotiate the
encryption keys. Maybe the first quantum-save secure VoIP calls ever ;-) .

While I was working on a modernisation of ZRTPCPP code base (using C++17 features, lots
of code cleanup etc.) I also had a look at the latest version of SIDH and this looks
promising. Thus I started to bring this SIDH version into the upcoming ZRTPCPP version
as an optional feature. ZRTPCPP will support SIDH P503 and SIDH P751 because of the
optimized ARM64 code. The other two SIDH types are too slow and add too much delay 
during setup of a VoIP call.

**NOTE:** All constraints regarding security apply - see original README info below.


## Cmake support

This fork of SIDH v3.2 contains `cmake` scripts to configure, compile and build the 
original SIDH C code a static libraries and also to build the original test programs. 
The cmake scripts use similar arguments to build the original libraries and test 
applications as the original Makefile. A typical setup to build SIDH libraries and test
applications using `cmake`:

    cd PQCrypto-SIDH
    mkdir cmake-build-debug
    cd cmake-build-debug
    cmake -Dx64=true ..     # Build X86_64 libraries and applications
    make

The `cmake-build-debug` sub-directories contain the static archives and the original
test applications. In addition `cmake` generates instructions to build a shared library,
see below.

Using `cmake` to build the static and shared libraries provides an easy way to generate
build instructions for Android libraries and `AAR` supporting `x86`, `x86_64`, `arm`,
and `arm64` 


## C++ wrapper and shared library

In this fork I added an C++ wrapper which provides a C++ interface to *all* SIDH types.
Thus if an application links the shared library and uses the C++ wrapper it may use all
four SIDH types without changing or linking other libraries.

Currently the wrapper supports the normal Diffie-Hellman only, not the compressed format
or the SIKE functions. These are planned, but not top priority at the moment.

The `unittests` folder contains tests (using the `gtest` framework) which show how to
use the C++ wrapper and tests the implementation.

To build a shared library with all four SIDH types I has to change the original code
to avoid duplicate global references. This was straight forward: just define most of the
C functions inside the the C source modules as `static`. This has no other side effects.

Another change was to use different names for some functions like `from_fp2mont`,
`from_mont`, and similar functions. See the files `P*_internal.h` in their respective
folders. This was done using appropriate `#define` statements, similar as it's done in
the `P*.c` C sources. 

Other than that no further changes to the original sources, no changes to the way the
functions perform the math :-) . 
 
**NOTE:** I had to disable the `__fast__` optimized code (assembler code) for the X64
architecture because the assembler code does not support position-independent code. The
offending lines are in file `fp_x64_asm.S` and look like this

        MUL128x256_SCHOOL [reg_p1], [rip+p434p1+24], r8, r9, r10, r11, r12, r13, rcx     

The culprit here is the reference to `p434p1` which is a global variable in the C-sources.
The same holds for `p503p1`, `p610p1`, and `p751p1` variables in their respective 
assembler files. Tested using clang/clang++ V9.0.1


## Android and Java/Kotlin support
 
The Android support consists of a few gradle files to build an `AAR` which contains the
shared libraries for the supported architectures and the Java/Kotlin interface to the
C++ wrapper using Java JNI. The `AAR` also contains the class files necessary to use
the libraries in an Android application.

The JNI interface to the C++ wrapper is generic and works with plain Java or Kotlin,
thus also outside an Android application.

Another folder contains a simple Android application which uses the SIDH AAR and provides
functions to select and run various SIDH tests on an Android device. Use Android studio
and import the project to build the AAR and the application. Also tested using gradle on
the command line.

The Android application project also contains some Junit tests to check the JNI interface.
See the standard `src/test/java` directory. Android Studio can run these tests without
an emulator or device attached. 
 
**NOTE:** I tested this on an Pixel 3 and it works quite well. Running the Android
application on the Android emulator (on my Linux, AMD X64 system) bails out
with an `illegal instruction` trap. Probably a missing instruction in the emulator.


## New content

* [Main cmake file](CMakeLists.txt) Contains the source reference, add sub directories etc
* [cmake file for gtest](CMakeLists_gtest.txt.in) Included by main cmake file to load
  gtest framework
* [top level gradle file](build.gradle) Sets up the gradle environment to support Android
  (other gradle related files are not listed here)
* [Android AAR module](AndroidSidhLib) Contains gradle files, resource to build the
  Android SIDH AAR
* [Android test application](AndroidSidhTest) Contains gradle files, sources etc to build
  the simple Android test application
* [C++ wrapper](cpp) Contains the C++ wrapper which is part of the shared library
* [Java/Kotlin JNI interface](cpp/javaNative) Java and C++ sources which implement the
  Java/Kotlin interface to the C++ wrapper
* [P434 specific cmake file](P434) Folder which contains the cmake file to build the 
  P434 specific test applications
* [P503 specific cmake file](P503) Folder which contains the cmake file to build the 
  P503 specific test applications
* [P610 specific cmake file](P610) Folder which contains the cmake file to build the
  P610 specific test applications
* [P751 specific cmake file](P751) Folder which contains the cmake file to build the
  P751 specific test applications
* [Modified files to run SIDH tests in Android test app](testsEmbedded) These files are
  copies of the respective files in `tests` folder, slightly modified to run inside
  another application, not as `main`.
   
Placing the specific cmake files in an own directory forces `cmake` to create the resulting
static test libraries and test application in separate directories and also keeps the
cmake caches and other build files separate.


# SIDH v3.4 (C Edition)

The **SIDH** library is an efficient supersingular isogeny-based cryptography library written in C language.
**Version v3.4** of the library includes the ephemeral Diffie-Hellman key exchange scheme "SIDH" [1,2], and the CCA-secure
key encapsulation mechanism "SIKE" [3]. These schemes are conjectured to be secure against quantum computer attacks.

Concretely, the SIDH library includes the following KEM schemes:

* SIKEp434: matching the post-quantum security of AES128 (level 1).
* SIKEp503: matching the post-quantum security of SHA3-256 (level 2).
* SIKEp610: matching the post-quantum security of AES192 (level 3).
* SIKEp751: matching the post-quantum security of AES256 (level 5).

And the following ephemeral key exchange schemes:

* SIDHp434: matching the post-quantum security of AES128 (level 1).
* SIDHp503: matching the post-quantum security of SHA3-256 (level 2).
* SIDHp610: matching the post-quantum security of AES192 (level 3).
* SIDHp751: matching the post-quantum security of AES256 (level 5).

It also includes the following compressed KEM schemes:

* SIKEp434_compressed: matching the post-quantum security of AES128 (level 1).
* SIKEp503_compressed: matching the post-quantum security of SHA3-256 (level 2).
* SIKEp610_compressed: matching the post-quantum security of AES192 (level 3).
* SIKEp751_compressed: matching the post-quantum security of AES256 (level 5).

And the following compressed ephemeral key exchange schemes:

* SIDHp434_compressed: matching the post-quantum security of AES128 (level 1).
* SIDHp503_compressed: matching the post-quantum security of SHA3-256 (level 2).
* SIDHp610_compressed: matching the post-quantum security of AES192 (level 3).
* SIDHp751_compressed: matching the post-quantum security of AES256 (level 5).

The compressed schemes exhibit reduced public keys at the expense of longer computing times.
Their implementation is based on [11,12], which in turn are based on and improves upon [9] and [10].

The library was developed by [Microsoft Research](http://research.microsoft.com/) for experimentation purposes.

## Contents

* [`KAT folder`](KAT/): Known Answer Test (KAT) files for the KEM.
* [`src folder`](src/): C and header files. Public APIs can be found in [`P434_api.h`](src/P434/P434_api.h), [`P503_api.h`](src/P503/P503_api.h), [`P610_api.h`](src/P610/P610_api.h) and [`P751_api.h`](src/P751/P751_api.h).
* [`Optimized x64 implementation for p434`](src/P434/AMD64/): optimized implementation of the field arithmetic over the prime p434 for x64 platforms.     
* [`Optimized x64 implementation for p503`](src/P503/AMD64/): optimized implementation of the field arithmetic over the prime p503 for x64 platforms.  
* [`Optimized x64 implementation for p610`](src/P610/AMD64/): optimized implementation of the field arithmetic over the prime p610 for x64 platforms. 
* [`Optimized x64 implementation for p751`](src/P751/AMD64/): optimized implementation of the field arithmetic over the prime p751 for x64 platforms.        
* [`Optimized ARMv8 implementation for p434`](src/P434/ARM64/): optimized implementation of the field arithmetic over the prime p434 for 64-bit ARMv8 platforms.   
* [`Optimized ARMv8 implementation for p503`](src/P503/ARM64/): optimized implementation of the field arithmetic over the prime p503 for 64-bit ARMv8 platforms.   
* [`Optimized ARMv8 implementation for p610`](src/P610/ARM64/): optimized implementation of the field arithmetic over the prime p610 for 64-bit ARMv8 platforms.    
* [`Optimized ARMv8 implementation for p751`](src/P751/ARM64/): optimized implementation of the field arithmetic over the prime p751 for 64-bit ARMv8 platforms.
* [`Generic implementation for p434`](src/P434/generic/): implementation of the field arithmetic over the prime p434 in portable C.
* [`Generic implementation for p503`](src/P503/generic/): implementation of the field arithmetic over the prime p503 in portable C.
* [`Generic implementation for p610`](src/P610/generic/): implementation of the field arithmetic over the prime p610 in portable C.
* [`Generic implementation for p751`](src/P751/generic/): implementation of the field arithmetic over the prime p751 in portable C.
* [`compression folder`](src/compression/): main C files of the compressed variants.
* [`random folder`](src/random/): randombytes function using the system random number generator.
* [`sha3 folder`](src/sha3/): SHAKE256 implementation.  
* [`Test folder`](tests/): test files.   
* [`Visual Studio folder`](Visual%20Studio/): Visual Studio 2015 files for compilation in Windows.
* [`Makefile`](Makefile): Makefile for compilation using the GNU GCC or clang compilers on Linux. 
* [`License`](LICENSE): MIT license file.
* [`Readme`](README.md): this readme file.

## Main Features

- Supports IND-CCA secure key encapsulation mechanism.
- Supports ephemeral Diffie-Hellman key exchange.
- Includes compressed variants that feature reduced public key sizes.
- Supports four security levels matching the post-quantum security of AES128, SHA3-256, AES192 and AES256.
- Protected against timing and cache-timing attacks through regular, constant-time implementation of 
  all operations on secret key material.
- Support for Windows OS using Microsoft Visual Studio, Linux OS and Mac OS X using GNU GCC and clang.     
- Provides basic implementation of the underlying arithmetic functions using portable C to enable support
  on a wide range of platforms including x64, x86, ARM and s390x. 
- Provides optimized implementations of the underlying arithmetic functions for x64 platforms with optional, 
  high-performance x64 assembly for Linux and Mac OS X. 
- Provides an optimized implementation of the underlying arithmetic functions for 64-bit ARM platforms using 
  assembly for Linux.
- Includes Known Answer Tests (KATs), and testing/benchmarking code.

## New in Version 3.3
 
- Improved versions of the four parameter sets for compressed SIDH and compressed SIKE [11,12].
- Optimized implementations of the field arithmetic for 64-bit ARMv8 processors for Linux.
- General optimizations to the field arithmetic.
- Support for Mac OS X for the optimized x64 assembly implementations.
- Support for big endian platforms, specifically IBM s390x processors.

## New in Version 3.4
 
- Memory optimizations for compressed SIDH and compressed SIKE.

## Supported Platforms

**SIDH v3.4** is supported on a wide range of platforms including x64, x86, ARM and s390x processors running Windows,
Linux or Mac OS X. We have tested the library with Microsoft Visual Studio 2015, GNU GCC v5.4, and clang v3.8.
See instructions below to choose an implementation option and compile on one of the supported platforms.

## Implementation Options

 The following implementation options are available:
- Portable implementations enabled by setting `OPT_LEVEL=GENERIC`. 
- Optimized x64 assembly implementations for Linux\Mac OS X enabled by setting `ARCH=x64` and `OPT_LEVEL=FAST`.
- Optimized ARMv8 assembly implementation for Linux enabled by setting `ARCH=ARM64` and `OPT_LEVEL=FAST`.

Follow the instructions in the sections "_Instructions for Linux_" or "_Instructions for Windows_" below to configure these different implementation options.

## Instructions for Linux

By simply executing:

```sh
$ make
```

the library is compiled for x64 using clang, optimization level `FAST`, and using the special instructions MULX
and ADX. Optimization level `FAST` enables the use of assembly, which in turn is a requirement to enable the 
optimizations using MULX/ADX.

Other options for x64:

```sh
$ make ARCH=x64 CC=[gcc/clang] OPT_LEVEL=[FAST/GENERIC] USE_MULX=[TRUE/FALSE] USE_ADX=[TRUE/FALSE]
```

When `OPT_LEVEL=FAST` (i.e., assembly use enabled), the user is responsible for setting the flags MULX and ADX 
according to the targeted platform (for example, MULX/ADX are not supported on Sandy or Ivy Bridge, only MULX 
is supported on Haswell, and both MULX and ADX are supported on Broadwell, Skylake and Kaby Lake architectures). 
Note that USE_ADX can only be set to `TRUE` if `USE_MULX=TRUE`.
The option `USE_MULX=FALSE` with `USE_ADX=FALSE` is only supported on p503 and p751.

Options for x86/ARM/s390x:

```sh
$ make ARCH=[x86/ARM/s390x] CC=[gcc/clang]
```

Options for ARM64:

```sh
$ make ARCH=[ARM64] CC=[gcc/clang] OPT_LEVEL=[FAST/GENERIC]
```

As in the x64 case, `OPT_LEVEL=FAST` enables the use of assembly optimizations on ARMv8 platforms.

Different tests and benchmarking results are obtained by running:

```sh
$ ./arith_tests-p434
$ ./arith_tests-p503
$ ./arith_tests-p610
$ ./arith_tests-p751
$ ./sike434/test_SIKE
$ ./sike503/test_SIKE
$ ./sike610/test_SIKE
$ ./sike751/test_SIKE
$ ./sidh434/test_SIDH
$ ./sidh503/test_SIDH
$ ./sidh610/test_SIDH
$ ./sidh751/test_SIDH
$ ./sike434_compressed/test_SIKE
$ ./sike503_compressed/test_SIKE
$ ./sike610_compressed/test_SIKE
$ ./sike751_compressed/test_SIKE
$ ./sidh434_compressed/test_SIDH
$ ./sidh503_compressed/test_SIDH
$ ./sidh610_compressed/test_SIDH
$ ./sidh751_compressed/test_SIDH
```

To run the KEM implementations against the KATs, execute:

```sh
$ ./sike434/PQCtestKAT_kem
$ ./sike503/PQCtestKAT_kem
$ ./sike610/PQCtestKAT_kem
$ ./sike751/PQCtestKAT_kem
$ ./sike434_compressed/PQCtestKAT_kem
$ ./sike503_compressed/PQCtestKAT_kem
$ ./sike610_compressed/PQCtestKAT_kem
$ ./sike751_compressed/PQCtestKAT_kem
```

The program tries its best at auto-correcting unsupported configurations. For example, since the `FAST` implementation is currently only available for x64 and ARMv8 doing `make ARCH=x86 OPT_LEVEL=FAST` is actually processed using `ARCH=x86 OPT_LEVEL=GENERIC`.

## Instructions for Windows

### Building the library with Visual Studio:

Open the solution file [`SIDH.sln`](Visual%20Studio/SIDH/SIDH.sln) in Visual Studio, choose either x64 or Win32 from the platform menu and then choose either `Fast` or `Generic` from the configuration menu (as explained above, the option `Fast` is not currently available for x86). Finally, select "Build Solution" from the "Build" menu. 

### Running the tests:

After building the solution file, there should be the following executable files: `arith_tests-P434.exe`, `arith_tests-P503.exe`, `arith_tests-P610.exe` and `arith_tests-P751.exe`, to run tests for the underlying arithmetic, `test-SIDHp[SET].exe` to run tests for the key exchange, and `test-SIKEp[SET].exe` to run tests for the KEM, where SET = {434, 503, 610, 751, 434_compressed, 503_compressed, 610_compressed, 751_compressed}.

### Using the library:

After building the solution file, add the generated `P434.lib`, `P503.lib`, `P610.lib` and `P751.lib` library files to the set of References for a project, and add [`P434_api.h`](src/P434/P434_api.h), [`P503_api.h`](src/P503/P503_api.h), [`P610_api.h`](src/P610/P610_api.h), [`P751_api.h`](src/P751/P751_api.h), [`P434_compressed_api.h`](src/P434/P434_compressed_api.h), [`P503_compressed_api.h`](src/P503/P503_compressed_api.h), [`P610_compressed_api.h`](src/P610/P610_compressed_api.h) and [`P751_compressed_api.h`](src/P751/P751_compressed_api.h) to the list of header files of a project.

## License

**SIDH** is licensed under the MIT License; see [`License`](LICENSE) for details.

The library includes some third party modules that are licensed differently. In particular:

- `tests/aes/aes_c.c`: public domain
- `tests/rng/rng.c`: copyrighted by Lawrence E. Bassham 
- `tests/PQCtestKAT_kem<#>.c`: copyrighted by Lawrence E. Bassham 
- `src/sha3/fips202.c`: public domain

## Contributors

- Basil Hess.
- Geovandro Pereira.
- Joost Renes.

# References

[1]  Craig Costello, Patrick Longa, and Michael Naehrig, "Efficient algorithms for supersingular isogeny Diffie-Hellman". Advances in Cryptology - CRYPTO 2016, LNCS 9814, pp. 572-601, 2016. 
The extended version is available [`here`](http://eprint.iacr.org/2016/413). 

[2]  David Jao and Luca DeFeo, "Towards quantum-resistant cryptosystems from supersingular elliptic curve isogenies". PQCrypto 2011, LNCS 7071, pp. 19-34, 2011. 
The extended version is available [`here`](https://eprint.iacr.org/2011/506).

[3]  Reza Azarderakhsh, Matthew Campagna, Craig Costello, Luca De Feo, Basil Hess, Amir Jalali, David Jao, Brian Koziel, Brian LaMacchia, Patrick Longa, Michael Naehrig, Joost Renes, Vladimir Soukharev, and David Urbanik, "Supersingular Isogeny Key Encapsulation". Submission to the NIST Post-Quantum Standardization project, 2017.  
The round 2 submission package is available [`here`](https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/SIKE-Round2.zip).

[4]  Craig Costello, and Huseyin Hisil, "A simple and compact algorithm for SIDH with arbitrary degree isogenies". Advances in Cryptology - ASIACRYPT 2017, LNCS 10625, pp. 303-329, 2017. 
The preprint version is available [`here`](https://eprint.iacr.org/2017/504). 

[5]  Armando Faz-Hernández, Julio López, Eduardo Ochoa-Jiménez, and Francisco Rodríguez-Henríquez, "A faster software implementation of the supersingular isogeny Diffie-Hellman key exchange protocol". IEEE Transactions on Computers, Vol. 67(11), 2018. 
The preprint version is available [`here`](https://eprint.iacr.org/2017/1015). 

[6]  Gora Adj, Daniel Cervantes-Vázquez, Jesús-Javier Chi-Domínguez, Alfred Menezes and Francisco Rodríguez-Henríquez, "On the cost of computing isogenies between supersingular elliptic curves". SAC 2018, LCNS 11349, pp. 322-343, 2018. 
The preprint version is available [`here`](https://eprint.iacr.org/2018/313). 

[7]  Samuel Jaques and John M. Schanck, "Quantum cryptanalysis in the RAM model: Claw-finding attacks on SIKE". Advances in Cryptology - CRYPTO 2019 (to appear), 2019. 
The preprint version is available [`here`](https://eprint.iacr.org/2019/103).   

[8]  Craig Costello, Patrick Longa, Michael Naehrig, Joost Renes and Fernando Virdia, "Improved Classical Cryptanalysis of the Computational Supersingular Isogeny Problem", 2019. 
The preprint version is available [`here`](https://eprint.iacr.org/2019/298). 

[9]  Craig Costello, David Jao, Patrick Longa, Michael Naehrig, Joost Renes and David Urbanik, "Efficient compression of SIDH public keys". Advances in Cryptology - EUROCRYPT 2017, LNCS 10210, pp. 679-706, 2017. 
The preprint version is available [`here`](https://eprint.iacr.org/2016/963). 

[10]  Gustavo H.M. Zanon, Marcos A. Simplicio Jr, Geovandro C.C.F. Pereira, Javad Doliskani and Paulo S.L.M. Barreto, "Faster key compression for isogeny-based cryptosystems". IEEE Transactions on Computers, Vol. 68(5), 2019. 
The preprint version is available [`here`](https://eprint.iacr.org/2017/1143).  

[11]  Michael Naehrig and Joost Renes, "Dual Isogenies and Their Application to Public-key Compression for Isogeny-based Cryptography". Advances in Cryptology - ASIACRYPT 2019, LNCS 11922, pp. 243-272, 2019.
The preprint version is available [`here`](https://eprint.iacr.org/2019/499).

[12]  Geovandro C.C.F. Pereira, Javad Doliskani and David Jao, "x-only point addition formula and faster torsion basis generation in compressed SIKE".
The preprint version is available [`here`](https://eprint.iacr.org/2020/431).

# Contributing

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
