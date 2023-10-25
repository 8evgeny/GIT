#include "main.h"
#include "widget.h"
#if 0

 #endif
#include <QApplication>
#include <QListWidget>

//static const QStringList LIST_ITEMS =
//    QStringList() << "C++" << "Python" << "Java" << "C#" << "PHP" << "Ruby" << "JavaScript";

//// Класс-обработчик сигналов от виджета списка
//class ListController : public QObject {
//    Q_OBJECT
//public slots:
//    void onListDoubleClicked( const QModelIndex& index ) {
//        if( !index.isValid() ) {
//            return;
//        }

//        if( QListWidget* listWgt = dynamic_cast< QListWidget* >( sender() ) ) {
//            if( QListWidgetItem* item = listWgt->takeItem( index.row() ) ) {
//                delete item;
//            }
//        }
//    }
//};


int main(int argc, char *argv[])
{
    QApplication app( argc, argv );

    Widget w;
    w.setWindowTitle("Создание комплекта документов на изделие");
    w.show();

//    QListWidget listWgt;
//    listWgt.addItems( LIST_ITEMS );
//    listWgt.resize( 300, 300 );
//    listWgt.show();

//    ListController listController;
//    QObject::connect(
//        &listWgt,
//        SIGNAL( doubleClicked( QModelIndex ) ),
//        &listController,
//        SLOT( onListDoubleClicked( QModelIndex ) )
//    );



//AppCore appCore;
//    appCore.readConfig("../config.ini");

return app.exec();
}
