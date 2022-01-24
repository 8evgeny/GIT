#include "classes/app.h"
#include "citgit_helper.h"

inline QStringList cidGtList() noexcept {
    QStringList list;
    auto const& cids = App::model()->cids();
    auto const& grs = App::model()->grs();
    std::transform(cids.cbegin(), cids.cend(), std::back_inserter(list), [] (auto cid) {
        return cidToString(cid);
    });
    std::transform(grs.cbegin(), grs.cend(), std::back_inserter(list), [] (auto gr) {
        return gtToString(gr);
    });
    return list;
}
