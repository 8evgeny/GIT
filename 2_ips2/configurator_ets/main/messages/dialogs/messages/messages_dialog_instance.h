#pragma once
#include <QHeaderView>
#include "include/delegates/play_delegate.h"
#include "include/delegates/checkbox_delegate.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "include/delegates/ip_delegate.h"

template <class U, class T, class Y>
class MessagesDialogInstanceType final {
 public:
    using class_name = MessagesDialogInstanceType;
    using ui_t       = U;
    using model_t    = T;
    using fmodel_t   = Y;
    using messages_t = MessagesDialog;

 private:
   void instanceWidgets() noexcept {       
       m_fmodel->setSourceModel(m_model);
       ui->m_view->setSortingEnabled(true);
       ui->m_view->setModel(m_fmodel);

       ui->m_view->setModel(m_model);
       ui->m_view->setSelectionMode(QAbstractItemView::SingleSelection);
       ui->m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
       ui->m_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
       ui->m_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
       ui->m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
       ui->m_view->horizontalHeader()->resizeSection(0, 100);
       ui->m_view->verticalHeader()->hide();
       ui->m_view->setShowGrid(false);
       //ui->m_view->setItemDelegateForColumn(0, new CheckBoxDelegate(ui->m_view));
       //ui->m_view->setItemDelegateForColumn(0, new IpDelegate(ui->m_view));
       ui->m_view->setItemDelegateForColumn(4, new PlayDelegate(m_dlg, ui->m_view));
   }

 public:
    explicit MessagesDialogInstanceType(ui_t* ui, model_t* model, fmodel_t* fmodel, messages_t* dlg)
        : ui(ui)
        , m_model(model)
        , m_fmodel(fmodel)
        , m_dlg(dlg)
   {
   }
    void operator()() noexcept {
       instanceWidgets();
    }

 private:
   ui_t* ui = nullptr;
   model_t* m_model = nullptr;
   fmodel_t* m_fmodel = nullptr;
   messages_t* m_dlg = nullptr;
};

template <class U, class T, class Y>
inline void messagesDialogInstance(U* ui, T* model, Y* fmodel, MessagesDialog* dlg) noexcept {
    MessagesDialogInstanceType<U, T, Y> instance(ui, model, fmodel, dlg);
    instance();
}
