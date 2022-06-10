#pragma once
#include <QDialog>
#include <QString>
#include "include/typedefs.h"

IPS_UI(DeleteTracksDialog)

class  DeleteTracksDialog: public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DeleteTracksDialog)

public:
    explicit DeleteTracksDialog(QString const& name = {}, QWidget* parent = nullptr);
    ~DeleteTracksDialog() override;

private:
    Ui::DeleteTracksDialog* ui = nullptr;
};
