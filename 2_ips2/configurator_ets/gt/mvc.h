/**
 * \file      surv/projects/cnf/include/mvc.h 
 * \brief     The Mvc class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2018 - 2020 
 * \version   v.1.1
 * \created   July      (the) 25(th), 2020, 17:16 MSK
 * \updated   March     (the) 27(th), 2021, 11:30 MSK
 * \TODO      
**/
#pragma once
#include <QListView>
#include <QModelIndex>

/** \namespace gt::mvc */
namespace gt::mvc {


/**
  \code
        return gt::mvc::isSelected(ui->m_view);
        return gt::mvc::isSelected(ui->m_model);
  \endcode
**/
template<class E>
static inline bool isSelected(E const& entity) noexcept {
    return entity.selectionModel()->currentIndex().isValid();
}
template<class E>
static inline bool isSelected(E* entity) noexcept {
    return entity->selectionModel()->currentIndex().isValid();
}

/**
  \code
        gt::mvc::setSelection(ui->m_view, selected(ui->m_view));
  \endcode
**/
template<class V>
static inline void setSelection(V* view, QModelIndex const& index
               , QItemSelectionModel::SelectionFlags flag = QItemSelectionModel::Select) noexcept {
    view->selectionModel()->setCurrentIndex(index, flag);
}

/**
  \code
        return gt::mvc::selected(ui->m_model);
        return gt::mvc::selected(ui->m_view);
  \endcode
**/
template<class E>
static inline QModelIndex selected(E* entity) noexcept {
    auto const& model = entity->selectionModel();
    return model->currentIndex();
}

/**
  \code
        auto model = gt::mvc::getModel<table_model_t>(ui->m_view);
  \endcode
 **/
template<class R, class E>
static inline R* getModel(E* entity) noexcept {
    return static_cast<R*>(entity->model());
}

/**
  \code
        auto model = gt::mvc::clickedItem(ui->m_view);
  \endcode
 **/
template<class V>
static inline QModelIndex clickedItem(V* view, QPoint const& pos) noexcept {
    return view->indexAt(pos);
}
template<class T>
static inline void selectRow(T* view, int row) noexcept {
    view->selectRow(row);
}
template<class T, class M>
static inline void selectFirstRow(T* view, M const& model) noexcept {
    view->setCurrentIndex(model.index(0,0));
}

} // end namespace gt::mvc
