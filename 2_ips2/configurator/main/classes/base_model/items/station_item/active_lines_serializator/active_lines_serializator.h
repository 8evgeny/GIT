#pragma once
#include <QByteArray>

class ActiveLinesSerializator
{
public:
    ActiveLinesSerializator();
    ~ActiveLinesSerializator() = default;

public:    
    void setSize(int size) noexcept;    
    QByteArray serializeActiveLines(QString const& line, uint key) noexcept;   
};
