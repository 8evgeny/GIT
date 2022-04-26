/*!
 \file trng.cpp

 \authors Shulenkov R.A.
*/
#include "trng.h"
#include "rs232.h"

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
            RS232::getInstance().term << "DeInitialization Error\n";
        }
    }

    /* Initialize the RNG peripheral */
    if (HAL_RNG_Init(&RngHandle) != HAL_OK) {
        /* Initialization Error */
        while (1) {
            RS232::getInstance().term << "Initialization Error\n";
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
        RS232::getInstance().term << "Generate random number Error\n";
    }
    return randomNum;
}

TRNG::TRNG()
{
    handle = &RngHandle;
}

void TRNG::test()
{
    RS232::getInstance().term << random() <<" - 1\n";
    RS232::getInstance().term << random() <<" - 2\n";
    RS232::getInstance().term << random() <<" - 3\n";
    RS232::getInstance().term << random() <<" - 4\n";
}
