/**
  ******************************************************************************
  * @file    AES_codec.h
  * @author  dmib@@ETS
  * @brief   This file contains AES coder and decoder module header
  ******************************************************************************
  */

#ifndef AES_CodecH
#define AES_CodecH

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @defgroup AES_module AES encryption module
  * @brief AES encryption and decryption functions
  * @{
  */

/** @defgroup AES_TYPE AES type constants
  * @brief  AES type constants
  * @{
  */
#define AES_128		128
#define AES_192		192
#define AES_256		256

/** 
  * @brief  selected AES type define
  */
#define AES_TYPE	AES_128

/**
  * @}
  */

/** 
  * @brief  AES block type constant
  */
#define AES_BLOCK_SIZE 16

/** 
  * @brief  AES state structure type definition
  */
typedef struct {
 uint8_t Nk;    // number words in key
 uint8_t Nr;    // number coderounds (depends on Nk)
 uint32_t ek[60];
 uint32_t dk[60];
} AES_State;

/** @defgroup AES_FUNC AES functions
  * @brief AES operation functions
  * @{
  */  
  
/**
  * @brief  AES keyset initialization
  * @param  [inout] st - AES state structure for keyset initialization.
  * @param  [in] key - array of bytes containing input AES key
  * @note   key array size must be according key size of AES type
  * @retval None
  */  
void AES_key_init(AES_State *st, uint8_t *key);
/**
  * @brief  AES encrypt array of data
  * @param  [in] st - AES state structure pointer
  * @param  [in] data_in - array of bytes for encrypting
  * @param  [out] data_out - array of encrypted data
  * @param  [in] len - size in bytes of data_in
  * @note   st must be already initialized
  * @note   len must be multiple of AES_BLOCK_SIZE
  * @note   size of data_out must be same as data_in
  * @retval None
  */  
void AES_code_data(AES_State *st, uint8_t *data_in, uint8_t *data_out, uint32_t len);
/**
  * @brief  AES decrypt array of data
  * @param  [in] st - AES state structure pointer
  * @param  [in] data_in - array of bytes for decrypting
  * @param  [out] data_out - array of decrypted data
  * @param  [in] len - size in bytes of data_in
  * @note   st must be already initialized
  * @note   len must be multiple of AES_BLOCK_SIZE
  * @note   size of data_out must be same as data_in
  * @retval None
  */  
void AES_decode_data(AES_State *st, uint8_t *data_in, uint8_t *data_out, uint32_t len);
  
/** 
  * @brief Get static AES state structure
  */
AES_State *AES_get_static_state();

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
