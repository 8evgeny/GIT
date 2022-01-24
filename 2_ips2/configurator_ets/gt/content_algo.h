#pragma once

// namespace gt::content
namespace gt::content {

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
 *  gt::content::create<DigitalStationForm>(ui, ui->w_content);
 *  gt::content::create(ui, ui->w_content);
**/
template<class T>
inline void create(T*& ui, QWidget* content) {
    gt::content::removeChildren(content);
    delete ui;
    ui = nullptr;
    ui = new T{};
    ui->setupUi(content);
}
} // end namespace gt::content
