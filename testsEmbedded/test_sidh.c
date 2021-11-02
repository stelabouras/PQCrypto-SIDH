/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: benchmarking/testing isogeny-based key exchange
*********************************************************************************************/ 


// Benchmark and test parameters  
#if defined(GENERIC_IMPLEMENTATION) || (TARGET == TARGET_ARM)
    #define BENCH_LOOPS        5      // Number of iterations per bench 
    #define TEST_LOOPS         5      // Number of iterations per test
#else
    #define BENCH_LOOPS       100       
    #define TEST_LOOPS        10      
#endif


static int cryptotest_kex()
{ // Testing key exchange
    unsigned int i;
    unsigned char PrivateKeyA[SIDH_SECRETKEYBYTES_A], PrivateKeyB[SIDH_SECRETKEYBYTES_B];
    unsigned char PublicKeyA[SIDH_PUBLICKEYBYTES], PublicKeyB[SIDH_PUBLICKEYBYTES];
    unsigned char SharedSecretA[SIDH_BYTES], SharedSecretB[SIDH_BYTES];
    bool passed = true;

    PRINTF("TESTING EPHEMERAL SIDH %s\n", SCHEME_NAME);
    PRINTF("------------------------------------------------------------------\n");

    for (i = 0; i < TEST_LOOPS; i++) 
    {
        random_mod_order_A(PrivateKeyA);
        random_mod_order_B(PrivateKeyB);

        EphemeralKeyGeneration_A(PrivateKeyA, PublicKeyA);                            // Get some value as Alice's secret key and compute Alice's public key
        EphemeralKeyGeneration_B(PrivateKeyB, PublicKeyB);                            // Get some value as Bob's secret key and compute Bob's public key
        EphemeralSecretAgreement_A(PrivateKeyA, PublicKeyB, SharedSecretA);           // Alice computes her shared secret using Bob's public key
        EphemeralSecretAgreement_B(PrivateKeyB, PublicKeyA, SharedSecretB);           // Bob computes his shared secret using Alice's public key
        
        if (memcmp(SharedSecretA, SharedSecretB, SIDH_BYTES) != 0) {
            passed = false;
            break;
        }
    }

    if (passed == true)
        PRINTF("  Key exchange tests ........................................... PASSED");
    else {
        PRINTF("  Key exchange tests ... FAILED"); PRINTF("\n"); return FAILED;
    }
    PRINTF("\n"); 

    return PASSED;
}


static int cryptorun_kex()
{ // Benchmarking key exchange
    unsigned int n;
    unsigned char PrivateKeyA[SIDH_SECRETKEYBYTES_A], PrivateKeyB[SIDH_SECRETKEYBYTES_B];
    unsigned char PublicKeyA[SIDH_PUBLICKEYBYTES], PublicKeyB[SIDH_PUBLICKEYBYTES];
    unsigned char SharedSecretA[SIDH_BYTES], SharedSecretB[SIDH_BYTES];
    unsigned long long cycles, cycles1, cycles2;

    PRINTF("\nBENCHMARKING EPHEMERAL SIDH %s (%d)\n", SCHEME_NAME, BENCH_LOOPS);
    PRINTF("------------------------------------------------------------------\n");

    random_mod_order_A(PrivateKeyA);
    random_mod_order_B(PrivateKeyB);

    // Benchmarking Alice's key generation
    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();
        EphemeralKeyGeneration_A(PrivateKeyA, PublicKeyA);
        cycles2 = cpucycles();
        cycles = cycles+(cycles2-cycles1);
    }
    PRINTF("  Alice's key generation runs in .... %10lld ", cycles/BENCH_LOOPS); print_unit;
    PRINTF("\n");

    // Benchmarking Bob's key generation
    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();
        EphemeralKeyGeneration_B(PrivateKeyB, PublicKeyB);
        cycles2 = cpucycles();
        cycles = cycles+(cycles2-cycles1);
    }
    PRINTF("  Bob's key generation runs in ...... %10lld ", cycles/BENCH_LOOPS); print_unit;
    PRINTF("\n");

    // Benchmarking Alice's shared key computation
    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();
        EphemeralSecretAgreement_A(PrivateKeyA, PublicKeyB, SharedSecretA); 
        cycles2 = cpucycles();
        cycles = cycles+(cycles2-cycles1);
    }
    PRINTF("  Alice's shared key comp. runs in .. %10lld ", cycles/BENCH_LOOPS); print_unit;
    PRINTF("\n");

    // Benchmarking Bob's shared key computation
    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();
        EphemeralSecretAgreement_B(PrivateKeyB, PublicKeyA, SharedSecretB);
        cycles2 = cpucycles();
        cycles = cycles+(cycles2-cycles1);
    }
    PRINTF("  Bob's shared key comp. runs in .... %10lld ", cycles/BENCH_LOOPS); print_unit;
    PRINTF("\n");

    return PASSED;
}


int DHTESTS()
{
    int Status = PASSED;
    
    Status = cryptotest_kex();             // Test key exchange
    if (Status != PASSED) {
        PRINTF("\n\n   Error detected: KEX_ERROR_SHARED_KEY \n\n");
        return FAILED;
    }

    Status = cryptorun_kex();              // Benchmark key exchange
    if (Status != PASSED) {
        PRINTF("\n\n   Error detected: KEX_ERROR_SHARED_KEY \n\n");
        return FAILED;
    }
    
    return Status;
}