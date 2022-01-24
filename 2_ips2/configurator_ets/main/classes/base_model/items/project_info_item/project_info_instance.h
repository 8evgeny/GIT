#pragma once
#include "project_number_validator.h"
#include "include/delegates/info_delegate.h"

#define STRING_MAX_LENGTH_COMMENTS 32

template <class U>
class ProjectInfoInstanceType {
    using class_name = ProjectInfoInstanceType;
    using ui_t       = U;

private:
    ui_t* ui = nullptr;

private:
   void instanceView() {
       ui->m_view->verticalHeader()->hide();
       ui->m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
       ui->m_view->setSelectionMode(QAbstractItemView::SingleSelection);
       ui->m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
       ui->m_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
       ui->m_number->setValidator(new ProjectNumberValidator(ui->m_number));
       auto delegate = new NameDelegate(STRING_MAX_LENGTH_COMMENTS);
       ui->m_view->setItemDelegateForColumn(1, delegate);
       ui->m_view->setItemDelegateForColumn(2, delegate);
       ui->m_view->horizontalHeader()->setHighlightSections(false);
//       ui->m_view->horizontalHeader()->setDefaultSectionSize(0);
//       ui->m_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
   }

public:
   explicit ProjectInfoInstanceType(ui_t* ui)
        : ui(ui) {
   }
   void operator()() {
       instanceView();
   }
};


template <class U>
inline void ProjectInfoInstance(U* ui) noexcept {
    ProjectInfoInstanceType<U> instance(ui);
    instance();
}
