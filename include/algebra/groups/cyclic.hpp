#pragma once

#include <cstdint>
#include <algebra/groups/element.hpp>

namespace algebra {

namespace detail {

template<std::uint32_t N>
struct CyclicModel {
    static_assert(N > 0);
    using storage_type = std::uint32_t;
    using descriptor = CyclicAdditiveGroupDescriptor;

    static storage_type zero_storage() {
        return 0;
    }

    static storage_type normalize_storage(storage_type value) {
        return value % N;
    }

    static storage_type from_integer_storage(std::int64_t value) {
        const auto modulus = static_cast<std::int64_t>(N);
        auto remainder = value % modulus;
        if (remainder < 0) {
            remainder += modulus;
        }
        return static_cast<storage_type>(remainder);
    }

    static storage_type add_storage(storage_type a, storage_type b) {
        std::uint64_t sum = static_cast<std::uint64_t>(a) + b;
        while (sum >= N) {
            sum -= N;
        }
        return static_cast<storage_type>(sum);
    }

    static storage_type negate_storage(storage_type value) {
        if (value == 0) {
            return 0;
        }
        return N - value;
    }

    static storage_type generator_storage() {
        return 1 % N;
    }

    static constexpr std::uint64_t cardinality() {
        return N;
    }
};

}

template<std::uint32_t N>
class Cyclic : public detail::CyclicModel<N>,
               public CyclicGroupStructure<AdditiveElement<detail::CyclicModel<N>, Cyclic<N>>> {
public:
    using Element = AdditiveElement<detail::CyclicModel<N>, Cyclic<N>>;

    static Element element(std::int64_t value) {
        return Element::from_integer(value);
    }
};

template<std::uint32_t N>
using C = Cyclic<N>;

}
