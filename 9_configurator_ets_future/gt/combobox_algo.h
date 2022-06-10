#pragma once
#include <QVariant>
#include <QComboBox>
#include "convert/enum.h"

/** \namespace gt::cb */
namespace gt::cb {

/**
 * \code
    #include "combobox_algo.h"
    auto const is = gt::cb::canSetCurrent(m_switcher.ui()->m_type, type);
 * \endcode
**/
template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
static inline bool canSetCurrent(QComboBox* box, T type) noexcept {
    return box->findData(convert::to_utype(type)) != -1;
}
template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0 >
static inline bool canSetCurrent(QComboBox* box, T val) noexcept {
    return box->findData(QVariant::fromValue<T>(val)) != -1;
}

/**
 * \code
    gt::cb::setCurrent(m_switcher.ui()->m_type, type);
 * \endcode
**/
template<class T>
static inline bool setCurrent(QComboBox* const box, T val) noexcept {
    int index = -1;
    if constexpr(std::is_enum_v<T>)
        index = box->findData(static_cast<int>(convert::to_utype(val)));
    else
        index = box->findData(QVariant::fromValue<T>(val));
    if(index == -1) return false;
    box->setCurrentIndex(index);
    return true;
}

/**
 * \code
    item.setType(gt::cb::current<item_t::type_t>((*ui)->m_type));
    auto const type = gt::cb::current<MuscleType>((*ui)->m_muscle);
 * \endcode
**/
template<class T>
static inline T current(QComboBox const* const box) noexcept {
    return box->currentData().value<T>();
}
} // end namespace gt::cb
