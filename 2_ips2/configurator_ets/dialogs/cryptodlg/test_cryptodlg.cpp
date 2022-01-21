#include <QString>
#include <QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>
//#include "cryptodlg.h"
#include "test_cryptodlg.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wconversion"
	#pragma GCC diagnostic warning "-Wsign-conversion"
	#pragma GCC diagnostic warning "-Wold-style-cast"
#elif defined(_MSC_VER)
#endif

uint8_t _key[16] = {0x2b, 0x7e, 0x15, 0x16,
				  0x28, 0xae, 0xd2, 0xa6,
				  0xab, 0xf7, 0x15, 0x88,
				  0x09, 0xcf, 0x4f, 0x3c};

 uint8_t Plain_Text[16] = {0x6b, 0xc1, 0xbe, 0xe2,
						 0x2e, 0x40, 0x9f, 0x96,
						 0xe9, 0x3d, 0x7e, 0x11,
						 0x73, 0x93, 0x17, 0x2a};

 uint8_t Cipher_Text[16] = {0x3a, 0xd7, 0x7b, 0xb4,
						 0x0d, 0x7a, 0x36, 0x60,
						 0xa8, 0x9e, 0xca, 0xf3,
						 0x24, 0x66, 0xef, 0x97};

uint8_t enc_buff[16];

Test_CryptoDialog::Test_CryptoDialog(QWidget *parent)
    : QObject(parent)
{
}

Test_CryptoDialog::~Test_CryptoDialog()
{
}


void Test_CryptoDialog::EncodeBuffer(){
	CryptoDialog crdlg;
	crdlg.setKey(_key, AES128);
	int _res = crdlg.EncodeBuffer(Plain_Text, 16, enc_buff);
	unsigned char _act, _exp;
	for(int _i = 0; _i < 16; _i++){
		_act = enc_buff[_i];
		_exp = Cipher_Text[_i];
		QCOMPARE(_act, _exp);
	}
	QCOMPARE(_res, 16); 
}

void Test_CryptoDialog::DecodeBuffer(){
	CryptoDialog crdlg;
	crdlg.setKey(_key, AES128);
	int _res = crdlg.DecodeBuffer(Cipher_Text, 16, enc_buff);
	for(int _i = 0; _i < 16; _i++)
		QCOMPARE(enc_buff[_i], Plain_Text[_i]);
	QCOMPARE(_res, 16); 
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
