# SIDH v3.4 C++ wrapper, shared library, Android, and Java/Kotlin support

## Foreword and disclaimer :-)
Back in 2017 Phil Zimmermann pointed me to the SIDH algorithm and I added SIDH to my
implementation of ZRTPCPP (available on github) as an enhancement to the normal
Diffie-Hellman key algorithms. Because SIDH was in a very early stage at that point
I just did as an experimental feature and used the *SilentPhone* Android client to
set-up ZRTP with SIDH and had some first VoIP calls using ZRTP/SIDH to negotiate the
encryption keys. Maybe the first quantum-save secure VoIP calls ever ;-) .

While I was working on a modernisation of ZRTPCPP code base (using C++17 features, lots
of code cleanup etc.) I also had a look at the latest version of SIDH and this looks
promising. Thus, I started to bring this SIDH version into the upcoming ZRTPCPP version
as an optional feature. 

**NOTE:** All constraints regarding security apply - see original README.


## Cmake support

This fork of SIDH v3.4 contains `cmake` scripts to configure, compile and build the
original SIDH C code into static libraries. The scripts also build the original test programs. 
A typical setup to build SIDH libraries and test applications using `cmake`:

    cd PQCrypto-SIDH
    mkdir cmake-build-debug
    cd cmake-build-debug
    cmake -Denable-x86_64=true -DBUILD_TESTS=true ..     # Build X86_64 libraries and applications
    make

The `cmake-build-debug` sub-directories contain the static archives and the original
test applications. In addition `cmake` generates instructions to build a shared library,
see below.

The `cmake` scripts contain build instructions for Android libraries and `AAR` supporting 
`x86`, `x86_64`, `arm`, and `arm64`. Refer to AndroidSidhLib/gradle.build. AndroidSidhTest
contains a small Android application which uses Android's SIDH libraries. 


## C++ wrapper and shared library

In this fork I added an C++ wrapper which provides a C++ interface to *all* SIDH types.
If an application links the shared library and uses the C++ wrapper it may use all
four SIDH types without changing or linking other libraries.

The wrapper supports the normal Diffie-Hellman only, not the compressed format
or the SIKE functions. These are planned, but not top priority at the moment.

The `unittests` folder contains tests (using the `gtest` framework) which show how to
use the C++ wrapper and tests the implementation.

To build a shared library with all four SIDH types I had to tweak the original code
to avoid duplicate global references. This was straight forward: just define several
C functions inside the C source modules as `static`. This has no other side effects.

Another change was to use different names for some functions like `from_fp2mont`,
`from_mont`, and similar functions. See the files `P*_internal.h` in their respective
folders. This was done using appropriate `#define` statements, similar as it's done in
the `P*.c` C sources.

Other than that no changes to the original sources.

**NOTE:** I had to disable the `__fast__` optimized code (assembler code) for the X64
architecture because the current assembler code does not support position-independent 
code. The offending lines are in file `fp_x64_asm.S` and look like this

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
functions to select and run various SIDH tests on an Android device. Use Android Studio
and import the project to build the AAR and the application. Also tested using gradle on
the command line.

The Android application project also contains some Junit tests to check the JNI interface.
See the standard `src/test/java` directory. Android Studio can run these tests without
an emulator or device attached.

**NOTE:** I tested this on an Pixel 3 and Pixel 5 and it works quite well. These run on 
ARM 64 bit CPU. Running SIDH on older ARM architecture would be too slow, not recommended.


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

