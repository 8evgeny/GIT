#include "active_lines_serializator.h"
#include "include/main_pch.h"

ActiveLinesSerializator::ActiveLinesSerializator()
{    
}
QByteArray ActiveLinesSerializator::serializeActiveLines(QString const& line, uint key) noexcept
{    
    if(line.isEmpty())
        return {};

    QRegExp rx(IPS_STR(R"([Cc][Ii][Dd]\d{1,}[\.]\d{1,}[;])"));
    rx.setMinimal(true);

    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);

    auto pos = 0;
    auto countMatches = 0;
    while ((pos = rx.indexIn(line, pos)) != -1) {
           auto const& cid = rx.cap(1).trimmed();
           auto const& destination = rx.cap(2).trimmed();

           out << static_cast<std::uint32_t>(cid.toInt());
           out << static_cast<std::uint32_t>(destination.toInt());
           out << static_cast<std::uint32_t>(key);
           out << static_cast<std::uint32_t>(countMatches);

           pos += rx.matchedLength();
           ++countMatches;
    }
    return arr;
}
