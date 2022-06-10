#pragma once
#include <QString>
#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QIcon>
#include <QPoint>
#include <QTableView>
#include <QPushButton>
#include <QStringList>
#include "include/view.h"

// namespace ips::algo
namespace ips::algo {

template <class T>
inline void removeChildren(QObject* parent) {
    while(auto w = parent->findChild<T>())
        delete w;
}
inline void removeChildren(QObject* parent) {
    while(auto w = parent->findChild<QObject*>())
        delete w;
}
/**
 *  ips::algo::createContent<DigitalStationForm>(ui, ui->w_content);
 *  ips::algo::createContent(ui, ui->w_content);
**/
template<class T>
inline void createContent(T*& ui, QWidget* content) {
    ips::algo::removeChildren(content);
    delete ui;
    ui = nullptr;
    ui = new T{};
    ui->setupUi(content);
}
template<class T>
inline void validation(const int min, const int max, T* t) {
    QIntValidator(min, max, t);
}
template<class T>
inline void clear(T* model)
{
    if (!model->items().isEmpty())
        while (model->items().size())
               model->removeRows(0, 1);
}
template<typename T>
inline void clearList(QList<T>& list) noexcept
{
    while (list.size())
            list.removeAt(0);

    list.clear();
}
inline void clearStringList(QStringList& list) noexcept
{
    while (list.size())
            list.removeAt(0);

    list.clear();
}
template <class T1, class T2>
void selectedRowDelete(T1* model, T2* view, int row) noexcept
{
    auto const& indx = mh::selected(view);
    if (!indx.isValid())
        return;

    model->removeRow(indx.row());
    mh::simple::view::selectAndClickRow(view, *model, (indx.row() == 0 ? 0 : indx.row()));

    if (row >= model->rowCount())
        mh::simple::view::selectAndClickRow(view, *model, (indx.row() == 0 ? 0 : indx.row()-1));
}
template <class T, class T1, class T2, class T3>
void contextMenu(T* model, T1* settings, T2* tableView, T3* addButton, T3* remButton, QString const& addText, QString const& remText, QString const& allText)
{
    QObject::connect(tableView, &QTableView::customContextMenuRequested, [model, settings, addText, remText, allText, addButton, remButton] (const QPoint &) {
        QMenu menu;
        auto add = menu.addAction(QIcon(IPS_STR(":/svg/plus_qmenu_dark.svg")), addText, [addButton]() {addButton->clicked();});
        auto rem = menu.addAction(QIcon(IPS_STR(":/svg/minus_qmenu_dark.svg")), remText, [remButton]() {remButton->clicked();});
        menu.addSeparator();
        auto remAll = menu.addAction(QIcon(IPS_STR(":/svg/basket_qmenu_dark.svg")), allText, [model, settings, addButton, remButton]() {
            model->removeRows(0, model->rowCount());
            remButton->setEnabled(model->rowCount());
            addButton->setEnabled(true);
            if (settings)
                settings->clear();
        });
        add->setEnabled(true);
        rem->setEnabled(model->rowCount());
        remAll->setEnabled(model->rowCount());
        menu.exec(QCursor::pos());
    });
}
} // end namespace ips::algo
