#pragma once
#include <QString>
#include "include/typedefs.h"
#include "main/classes/duration.h"

class Pretone final {
 public:
     using class_name = Pretone;

 public:
    explicit Pretone(uint id = 0, duration_t duration = duration_t::ONCE)
        : m_id(id)
        , m_duration(duration) {
    }
    ~Pretone() = default;

 public:
    constexpr void setId(uint id) noexcept {
        m_id = id;
    }
    constexpr auto id() const noexcept {
        return m_id;
    }
    constexpr auto duration() const noexcept {
        return m_duration;
    }
    constexpr void setDuration(duration_t const& duration) noexcept {
        m_duration = duration;
    }
    constexpr friend bool operator==(class_name const& lhs, class_name const& rhs) noexcept {
        return lhs.id() == rhs.id()
            && lhs.duration() == rhs.duration();
    }
    constexpr friend bool operator!=(class_name const& lhs, class_name const& rhs) noexcept {
        return !(lhs == rhs);
    }    

 private:
    uint m_id;    
    duration_t m_duration = duration_t::ONCE;
};
