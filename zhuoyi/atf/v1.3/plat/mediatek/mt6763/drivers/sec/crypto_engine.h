#ifndef __CRYPTO_ENGINE_H_
#define __CRYPTO_ENGINE_H_

#define CRYPTO_ENGINE_SW      1
#define CRYPTO_ENGINE_GCPU    2
#define CRYPTO_ENGINE_TZCC    3

#define CRYPTO_ENGINE_SHA_SW_SUPPORT      0
#define CRYPTO_ENGINE_SHA_GCPU_SUPPORT    0
#define CRYPTO_ENGINE_SHA_TZCC_SUPPORT    1

#define CRYPTO_ENGINE_RSA_SW_SUPPORT      0
#define CRYPTO_ENGINE_RSA_GCPU_SUPPORT    0
#define CRYPTO_ENGINE_RSA_TZCC_SUPPORT    1

#define CRYPTO_ENGINE_SHA                 CRYPTO_ENGINE_TZCC
#define CRYPTO_ENGINE_RSA                 CRYPTO_ENGINE_TZCC

#if CRYPTO_ENGINE_SHA == CRYPTO_ENGINE_GCPU
	#if !CRYPTO_ENGINE_SHA_GCPU_SUPPORT
	#error CRYPTO_ENGINE_SHA_GCPU_SUPPORT = 0 on current platform!!
	#endif
#elif CRYPTO_ENGINE_SHA == CRYPTO_ENGINE_TZCC
	#if !CRYPTO_ENGINE_SHA_TZCC_SUPPORT
	#error CRYPTO_ENGINE_SHA_TZCC_SUPPORT = 0 on current platform!!
	#endif
#endif

#if CRYPTO_ENGINE_RSA == CRYPTO_ENGINE_GCPU
	#if !CRYPTO_ENGINE_RSA_GCPU_SUPPORT
	#error CRYPTO_ENGINE_RSA_GCPU_SUPPORT = 0 on current platform!!
	#endif
#elif CRYPTO_ENGINE_RSA == CRYPTO_ENGINE_TZCC
	#if !CRYPTO_ENGINE_RSA_TZCC_SUPPORT
	#error CRYPTO_ENGINE_RSA_TZCC_SUPPORT = 0 on current platform!!
	#endif
#endif

#endif /* __CRYPTO_ENGINE_H_ */
