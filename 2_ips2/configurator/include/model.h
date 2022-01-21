#pragma once
#include "main_pch.h"

/** \namespace mh */
namespace mh {

/**
  \code
        return mh::isSelected(ui->m_table);
        return mh::isSelected(ui->m_model);
  \endcode
**/
template<class E>
static inline bool isSelected(E const& entity) Q_DECL_NOEXCEPT {
    return entity.selectionModel()->currentIndex().isValid();
}
template<class E>
static inline bool isSelected(E* entity) Q_DECL_NOEXCEPT {
    return entity->selectionModel()->currentIndex().isValid();
}
/**
  \code
        return mh::setSelection(ui->m_table, selected(ui->m_table), QItemSelection::Clear);
  \endcode
**/
template<class T>
static inline void setSelection(T* table, QModelIndex const& index
               , QItemSelectionModel::SelectionFlags flag = QItemSelectionModel::Select) Q_DECL_NOEXCEPT {
    table->selectionModel()->setCurrentIndex(index, flag);
}
template<class T>
static inline void setUnSelection(T* table, QModelIndex const& index) Q_DECL_NOEXCEPT {
    setSelection(table, index, QItemSelectionModel::Deselect);
}

/**
  \code
        return mh::selected(ui->m_model);
        return mh::selected(ui->m_table);
  \endcode
**/
template<class E>
static inline QModelIndex selected(E* entity) Q_DECL_NOEXCEPT {
    auto const& model = entity->selectionModel();
    return model->currentIndex();
}

/**
  \code
        auto model = mh::getModel<view_model_t>(ui->m_model);
        auto model = mh::getModel<table_model_t>(ui->m_table);
  \endcode
 **/
template<class R, class E>
static inline R* getModel(E* entity) Q_DECL_NOEXCEPT {
    return static_cast<R*>(entity->model());
}

} // end namespace mh::model
