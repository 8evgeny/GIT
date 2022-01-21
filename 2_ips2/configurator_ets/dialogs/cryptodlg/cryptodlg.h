/*!
 * @file cryptodlg.h
 * @file cryptodlg.cpp
 * @brief This file includes the class of the encryption array data 
 */

/*! 
\defgroup crypto_module the class of the encryption array data 
*/
#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "cryptodlg_pch.h"
#include "cryptodlg_global.h"

/*! @struct AES_State 
*  @brief structure describes the encryption state of the buffer 
*/
/*! @var AES_State::Nk
*  Member 'Nk' contains number words in key
*/
/*! @var AES_State::Nr
*  Member 'Nr' contains number coderounds (depends on Nk)
*/
/*! @var AES_State::ek
*  Member 'ek' contains array encode data buffer
*/
/*! @var AES_State::dk
*  Member 'dk' contains array decode data buffer
*/
typedef struct {
 uint8_t Nk;
 uint8_t Nr;
 uint32_t ek[60];
 uint32_t dk[60];
} AES_State;
/*! @enum QCryptoType 
* @brief enumeration of encryption types
* @var QCryptoType::AES128
*  Member 'AES128' description 128-bit key
* @var QCryptoType::AES192
*  Member 'AES192' description 192-bit key
* @var QCryptoType::AES256
*  Member 'AES256' description 256-bit key
*/
typedef enum{
	AES128 = 0, AES192, AES256
}QCryptoType;

//#define __IS_PHANTOM_ENC__

class CRYPTO_DIALOG_SHARED_EXPORT CryptoDialog final: public QObject {
    Q_OBJECT
//    IPS_DISABLE_COPY(CryptoDialog)

 public:
    using inherited = QObject;
    using class_name = CryptoDialog;
/*!
\fn void setKey(uint8_t* key, QCryptoType type)
\brief Function set encryption key
\param [in] key		pointer to array of symbols encryption key
\param [in] type	type key encryption
*/
	void setKey(uint8_t* key, QCryptoType type);
/*!
\fn EncodeBuffer(uint8_t *src_buff, uint32_t length_src_buff, uint8_t *dst_buff)
\brief Function encryption data buffer
\param [in] src_buff			pointer to source buffer
\param [in] length_src_buff		length of source buffer
\param [out] dst_buff			pointer to destination buffer
\return length of destination buffer
*/
	uint32_t EncodeBuffer(uint8_t *src_buff, uint32_t length_src_buff, uint8_t *dst_buff);
/*!
\fn DecodeBuffer(uint8_t *src_buff, uint32_t length_src_buff, uint8_t *dst_buff)
\brief Function decryption data buffer
\param [in] src_buff			pointer to source buffer
\param [in] length_src_buff		length of source buffer
\param [out] dst_buff			pointer to destination buffer
\return length of destination buffer
*/
	uint32_t DecodeBuffer(uint8_t *src_buff, uint32_t length_src_buff, uint8_t *dst_buff);
 public:
/*!
\fn CryptoDialog(QWidget* parent = nullptr)
\brief Function creation class
\param[in] parent pointer to the parent class
*/
    explicit CryptoDialog(QWidget* parent = nullptr);
/*!
 \fn ~CryptoDialog()
 \brief Function destroy class
*/
    ~CryptoDialog() override;
private:
///type key encryption
	QCryptoType typeCrypto;
///array symbol encryption key
	uint8_t CryptoKey[32];

private:
/*!
\fn uint8_t KeyInit(uint8_t *key, uint8_t keyLength, AES_State *st)
\brief Function initialize encryption key
\param[in] key			pointer to array of symbols encryption key
\param[in] keyLength	length encryption key
\param[out] st			pointer to state
\return result initialize
*/
	uint8_t KeyInit(uint8_t *key, uint8_t keyLength, AES_State *st);
/*!
\fn void Code_Block(uint8_t *block, AES_State *st)
\brief Function code block data
\param[in/out] block	pointer to block data
\param[out] st			pointer to state
*/
	void Code_Block(uint8_t *block, AES_State *st);
/*!
\fn void Decode_Block(uint8_t *block, AES_State *st)
\brief Function code block data
\param[in/out] block	pointer to block data
\param[out] st			pointer to state
*/
	void Decode_Block(uint8_t *block, AES_State *st);
};
