#pragma once
#include <QString>
#include "main/classes/algo.h"
#include "include/typedefs.h"
#include "classes/app.h"
#include "messages/dialogs/messages/messages_dialog.h"
#include "messages/model_tracks/track.h"
#include <run_tests/run_tests.h>

class MessDlgTest {

    using class_name = MessDlgTest;
    using dlg_name = MessagesDialog;

public:
    MessDlgTest(dlg_name* dlg = nullptr)
        : m_dlg(dlg) {}

public:
    dlg_name* mess() noexcept {
        return m_dlg;
    }
    void addItems(int count) noexcept {
        for (auto i=0; i < count; ++i) {
            Track track(QString::number(i));
            m_dlg->model()->insertItem(i, track);
        }
    }

private:
    dlg_name* m_dlg = nullptr;
};
