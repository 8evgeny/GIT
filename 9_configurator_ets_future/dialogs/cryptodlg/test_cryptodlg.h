/*!
 * @file test_cryptodlg.h
 * @file test_cryptodlg.cpp
 * @brief This file includes the class of the testing encryption array data 
 */

/*! 
\defgroup test_crypto_module the class of the testing encryption array data 
*/
#ifndef TEST_CRYPTODLG_H
#define TEST_CRYPTODLG_H

#pragma once
//#include "include/typedefs.h"
#include "string.h"
#include <stdint.h>
#include "QTest"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "cryptodlg.h"

class Test_CryptoDialog final: public QObject {
    Q_OBJECT

 public:
    using inherited = QObject;
    using class_name = Test_CryptoDialog;
 public:
/*!
\fn Test_CryptoDialog(QWidget* parent = nullptr)
\brief Function creation class
\param[in] parent pointer to the parent class
*/
    explicit Test_CryptoDialog(QWidget* parent = nullptr);
/*!
 \fn ~CryptoDialog()
 \brief Function destroy class
*/
    ~Test_CryptoDialog() override;

private slots:
/*!
\fn EncodeBuffer()
\brief Function testing encryption data buffer
*/
	void EncodeBuffer();
/*!
\fn DecodeBuffer()
\brief Function testing decryption data buffer
*/
    void DecodeBuffer();
};
#endif
