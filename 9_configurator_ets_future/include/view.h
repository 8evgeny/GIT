#pragma once
#include <QListView>
#include <QModelIndex>
#include <QTableView>
#include "model.h"

/** \namespace mh::simple::view */
namespace mh::simple::view {

template<class T>
static inline void setCurrentIndex(T* view, QModelIndex const& index) noexcept {
    view->setCurrentIndex(index);
}
template<class T>
static inline void selectRow(T* view, int row) noexcept {
    view->selectRow(row);
}
template<class V, class M>
static inline void deleteRow(V* view, M&& model) noexcept {
    auto const& indx = mh::selected(view);
    if (indx.isValid()) {
        model.removeRows(indx.row(), 1);
        auto const row = indx.row() == 0 ? 0 : indx.row() - 1;
        selectRow(view, row);
    }
}

inline void selectRow(QListView* view, int row) noexcept {
    setCurrentIndex(view, view->model()->index(row,0));
}
inline void selectRow(QTreeView* view, int row) noexcept {
    setCurrentIndex(view, view->model()->index(row,0));
}
inline void selectRow(QTableView* view, int row) noexcept {
    setCurrentIndex(view, view->model()->index(row,0));
}

/**
 * \code
        msv::selectFirstRow(ui->m_view, ui->m_model);
 * \endcode
**/
template<class V, class M>
static inline void selectFirstRow(V* view, M&& model) noexcept {
    if(std::forward<M>(model).rowCount() < 1)
        return;

    setCurrentIndex(view,  std::forward<M>(model).index(0,0));
    selectRow(view, 0);
}
template<class V, class M>
static inline void selectAndClickRow(V* view, M&& model, int row) noexcept {
    if(std::forward<M>(model).rowCount() <= row || row < 0)
        return;

    setCurrentIndex(view, std::forward<M>(model).index(row,0));
    selectRow(view, row);
    view->clicked(std::forward<M>(model).index(row, 0));
}
template<class V, class M>
static inline void selectAndClickFirstRow(V* view, M&& model) noexcept {
    if(std::forward<M>(model).rowCount() < 1)
        return;

    setCurrentIndex(view, std::forward<M>(model).index(0,0));
    selectRow(view, 0);
    view->clicked(std::forward<M>(model).index(0, 0));
}

} // end namespace mh::simple::view
