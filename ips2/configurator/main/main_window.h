#pragma once
#include "include/statusbar.h"
#include "classes/app.h"
#include <QPoint>
#include <QFile>

IPS_UI(MainWindow)

class MainWindow final: public QMainWindow {
    Q_OBJECT
    IPS_DISABLE_COPY(MainWindow)
    friend class TestMainWindow;

public:
    using inherited = QMainWindow;
    using ui_m      = Ui::MainWindow;
    using statusBar = QStatusBar;

public:
    explicit MainWindow(App* app = nullptr, QWidget* parent = nullptr);
    ~MainWindow() final;
     void changeEvent(QEvent* event) final;

private:
    void initSignals();
    void createMenuFile();
    void createMenuInstruments();     
    bool checkHashProject();
    bool checkSaveProject(bool is = false);
    void updateUserFile() noexcept;
    void loadProject(QString const& file);    
    void initRecentProjects(QMenu& menu, QMenu* sub) noexcept;

private slots:        
    void onTreeViewClicked(const QModelIndex& index);
    void loadProject();
    void saveProject();
    bool saveProjectAs();    

private:
    unsigned char* m_dest = nullptr;
    unsigned long* m_destLen = nullptr;

    unsigned char* m_destUn = nullptr;
    unsigned long* m_destLenUn = nullptr;

private:    
    bool m_screenState = true;
    bool m_StationItem = false;
    QPoint m_point;
    ui_m* ui = nullptr;
    QString m_file;        
    QModelIndex m_index = {};
    StatusBar* m_bar= nullptr;
    statusBar* m_status = nullptr;    
};
