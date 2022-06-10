/*!
 \file trng.cpp

 \authors Shulenkov R.A.
*/
#include "trng.h"
#include "../Debug/debug.h"

TRNG *TRNG::pInstance = nullptr;
TRNGDestroyer TRNG::destroyer;

TRNGDestroyer::~TRNGDestroyer()
{
    delete pInstance;
}

void TRNGDestroyer::initialize(TRNG *p)
{
    pInstance = p;
}

TRNG &TRNG::getInstance()
{
    if (!pInstance)     {
        pInstance = new TRNG();
        destroyer.initialize(pInstance);
    }
    return *pInstance;
}

#ifdef __cplusplus
extern "C" {
#endif

static RNG_HandleTypeDef RngHandle;

void rngInit(void)
{
    RngHandle.Instance = RNG;

    /* DeInitialize the RNG peripheral */
    if (HAL_RNG_DeInit(&RngHandle) != HAL_OK) {
        /* DeInitialization Error */
        while (1) {
            Debug::getInstance().dbg << "DeInitialization Error\n";
        }
    }

    /* Initialize the RNG peripheral */
    if (HAL_RNG_Init(&RngHandle) != HAL_OK) {
        /* Initialization Error */
        while (1) {
            Debug::getInstance().dbg << "Initialization Error\n";
        }
    }
}

#ifdef __cplusplus
}
#endif

uint32_t TRNG::random()
{
    uint32_t randomNum = 0;
    while (HAL_RNG_GenerateRandomNumber(handle, &randomNum) != HAL_OK) {
        Debug::getInstance().dbg << "Generate random number Error\n";
    }
    return randomNum;
}

TRNG::TRNG()
{
    handle = &RngHandle;
}

void TRNG::test()
{
    Debug::getInstance().dbg << random() <<" - 1\n";
    Debug::getInstance().dbg << random() <<" - 2\n";
    Debug::getInstance().dbg << random() <<" - 3\n";
    Debug::getInstance().dbg << random() <<" - 4\n";
}
