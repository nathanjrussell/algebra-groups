#pragma once

#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <algebra/groups/cyclic.hpp>
#include <algebra/groups/direct_sum.hpp>

namespace algebra {

template<std::uint32_t A, std::uint32_t B>
class ChineseRemainderIsomorphism {
    static_assert(A > 0 && B > 0);
    static_assert(std::gcd(A, B) == 1);
    static constexpr std::uint64_t product = static_cast<std::uint64_t>(A) * B;
    static_assert(product <= std::numeric_limits<std::uint32_t>::max());

public:
    using Source = Cyclic<static_cast<std::uint32_t>(product)>;
    using Target = AbelianGroup<DirectSum<Cyclic<A>, Cyclic<B>>>;

    static typename Target::Element forward(const typename Source::Element& value) {
        const auto representative = value.representative();
        return Target::element({representative, representative});
    }

    static typename Source::Element backward(const typename Target::Element& value) {
        const auto a = std::get<0>(value.storage()).representative();
        const auto b = std::get<1>(value.storage()).representative();
        for (std::uint64_t candidate = 0; candidate < product; ++candidate) {
            if (candidate % A == a && candidate % B == b) {
                return Source::element(static_cast<std::int64_t>(candidate));
            }
        }
        throw std::logic_error("No representative satisfies the coordinates");
    }
};

}
