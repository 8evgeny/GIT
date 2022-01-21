/**
 * @file exampleitem.h
 * @brief Заголовочный файл класса ExampleItem
 */
#ifndef EXAMPLEITEM_H
#define EXAMPLEITEM_H
 
#include <QString>
#include <QTextCodec>

 /**
  * \class ExampleItem exampleitem.h "src/exampleitem.h"
  * \brief Класс, содержащий определение одной строки модели отображения данных командного документирования
  */
class ExampleItem
{
public:
/*!
* \fn ExampleItem();
* \brief Конструктор класса
*/
    ExampleItem();
	QString index;		/*!< Индекс сообщения */
    QString datetime;	/*!< Дата-Время наступления события */
    QString description;/*!< Краткое описание команды события */
    QString voiceuid;	/*!< Идентификатор речевого документирования */
	QString source;		/*!< Исходный код кодограммы события командного документирования */
};
 
#endif // EXAMPLEITEM_H
