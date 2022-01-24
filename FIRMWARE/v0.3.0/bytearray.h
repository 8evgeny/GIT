#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include "QByteArray"

class ByteArray : public QByteArray
{
public:
	ByteArray() : QByteArray() { }
	ByteArray(const char *data) : QByteArray(data) { }
	ByteArray(const char *data, int size) : QByteArray(data, size) { }
	ByteArray(int size, char c) : QByteArray(size, c) { }
	ByteArray(int size, Qt::Initialization i) : QByteArray(size, i) {}
	ByteArray(const QByteArray &other) : QByteArray(other) { }
	~ByteArray() {}
	inline ByteArray &operator=(const char * str) { return (ByteArray &)QByteArray::operator=(str); }
	inline ByteArray &append(const QByteArray &other) { QByteArray::append(other); return *this; }
	//inline ByteArray &append(const  ByteArray &other) { QByteArray::append(other.q()); return *this; }
	inline ByteArray &append(const qint64  i, int n) { QByteArray::append((const char *)&i, n); return *this; }
	inline ByteArray &append(const quint64 i, int n) { QByteArray::append((const char *)&i, n); return *this; }
	inline ByteArray &append(const qint64  i) { QByteArray::append((const char *)&i, 8); return *this; }
	inline ByteArray &append(const quint64 i) { QByteArray::append((const char *)&i, 8); return *this; }
	inline ByteArray &append(const qint32  i) { QByteArray::append((const char *)&i, 4); return *this; }
	inline ByteArray &append(const quint32 i) { QByteArray::append((const char *)&i, 4); return *this; }
	inline ByteArray &append(const qint16  i) { QByteArray::append((const char *)&i, 2); return *this; }
	inline ByteArray &append(const quint16 i) { QByteArray::append((const char *)&i, 2); return *this; }
	inline ByteArray &append(const qint8   i) { QByteArray::append((const char *)&i, 1); return *this; }
	inline ByteArray &append(const quint8  i) { QByteArray::append((const char *)&i, 1); return *this; }
	
	inline QByteArray &q() { return *this; }
	//inline QByteArray toHex() const { return ((QByteArray *)this)->QByteArray::toHex(); } сегфолтится т.к. рекурсивно вызывает саму себя не пойму какого хрена
	
	ByteArray &appendCrc16CCITT();
	ByteArray &revers();
	static void test();
};

#endif // BYTEARRAY_H
