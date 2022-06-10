/**
 * @file exampletablemodel.h
 * @file exampletablemodel.cpp
 * @brief Заголовочный файл класса ExampleTableModel
 */
#ifndef EXAMPLETABLEMODEL_H
#define EXAMPLETABLEMODEL_H
 
#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include "exampleitem.h"
/**
* \class ExampleTableModel exampletablemodel.h "src/exampletablemodel.h"
* \brief Класс, содержащий модель отображения данных командного документирования
*/
class ExampleTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
/*!
* \fn explicit ExampleTableModel(QObject *parent = 0);
* \brief Конструктор класса
* \param parent Указатель на родителя класса
*/
    explicit ExampleTableModel(QObject *parent = 0);
/*!
* \fn void AppendRow(ExampleItem* _item);
* \brief Функция добавления одной строки о событии командного документирования
* \param _item Указатель элемент класса @ref ExampleItem - одна строка, строка содержащая данные о событии командного документирования
* \return void
*/
	void AppendRow(ExampleItem* _item);
/*!
* \fn QVariant data(const QModelIndex &index, int role) const;
* \brief Функция возвращения содержимого ячейки выделенной строки модели
* \param index индех модели
* \param role режим модели
* \return QVariant
*/
	QVariant data(const QModelIndex &index, int role) const;
/*!
* \fn ExampleItem data(int row);
* \brief Функция возвращения содержимого выделенной строки модели
* \param row номер выделенной строки
* \return @ref ExampleItem
*/
	ExampleItem data(int row);
/*!
* \fn bool setData(const QModelIndex &index, const QVariant &value, int role);
* \brief Функция вставки содержимого ячейки модели
* \param index индех модели
* \param role режим модели
* \param value содержимое ячейки
* \return bool результат вставки
*/
    bool setData(const QModelIndex &index, const QVariant &value, int role);
/*!
* \fn int rowCount(const QModelIndex &parent=QModelIndex()) const;
* \brief Функция возвращает количество строк модели
* \param parent индех модели
* \return int результат
*/
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
/*!
* \fn int columnCount(const QModelIndex &parent) const;
* \brief Функция возвращает количество колонок модели
* \param parent индех модели
* \return int результат
*/
    int columnCount(const QModelIndex &parent) const;
/*!
* \fn void clear();
* \brief Функция очистки модели
* \return void
*/
	void clear();
/*!
* \fn void MyReset();
* \brief Функция обновления(пересброса) модели
* \return void
*/
	void MyReset();
/*!
* \fn QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
* \brief Функция возвращает содержимое заголовка ячейки модели
* \param section номер секции
* \param orientation режим ориентации заголовка
* \param role режим работы заголовка
* \return QVariant
*/
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
/*!
* \fn Qt::ItemFlags flags(const QModelIndex &index) const;
* \brief Функция возвращает флаг состояния модели
* \param index индекс модели
* \return Qt::ItemFlags
*/
    Qt::ItemFlags flags(const QModelIndex &index) const;
/*!
* Список сообщений
*/
    QList<ExampleItem *> list;
/*!
* Список элементов заголовка модели
*/
    QStringList header_data;
 };
#endif // EXAMPLETABLEMODEL_H
