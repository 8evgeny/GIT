#pragma once
#include <QStringList>
#include "classes/app.h"

inline auto cidGtList() noexcept {
    QStringList list;    
    auto const& cids = App::model()->cids();    
    auto const& grs = App::model()->gids();
    std::transform(grs.cbegin(), grs.cend(), std::back_inserter(list), [] (auto gr) {
        return gtToString(gr);
    });
    std::transform(cids.cbegin(), cids.cend(), std::back_inserter(list), [] (auto cid) {
        return cidToString(cid);
    });    
    return list;
}
inline auto cidList() noexcept {
    QStringList list;
    auto const& cids = App::model()->cids();
    std::transform(cids.cbegin(), cids.cend(), std::back_inserter(list), [] (auto cid) {
        return cidToString(cid);
    });
    return list;
}
