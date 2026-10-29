#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <algebra/groups/element.hpp>

namespace algebra {

template<class G>
concept AdditiveAbelianGroupModel = requires { typename G::Element; } &&
    AbelianGroupElement<typename G::Element, Addition>;

template<AdditiveAbelianGroupModel... Factors>
struct DirectSum {
    using storage_type = std::tuple<typename Factors::Element...>;
    using descriptor = AdditiveGroupDescriptor;

    static storage_type zero_storage() {
        return storage_type{Factors::Element::zero()...};
    }

    static storage_type normalize_storage(storage_type value) {
        return value;
    }

    template<std::size_t... I>
    static storage_type add_impl(const storage_type& a, const storage_type& b,
                                 std::index_sequence<I...>) {
        return storage_type{std::get<I>(a).add(std::get<I>(b))...};
    }

    static storage_type add_storage(const storage_type& a, const storage_type& b) {
        return add_impl(a, b, std::index_sequence_for<Factors...>{});
    }

    template<std::size_t... I>
    static storage_type negate_impl(const storage_type& a, std::index_sequence<I...>) {
        return storage_type{std::get<I>(a).negate()...};
    }

    static storage_type negate_storage(const storage_type& a) {
        return negate_impl(a, std::index_sequence_for<Factors...>{});
    }

    template<std::size_t... I>
    static storage_type coordinates_impl(const std::array<std::int64_t, sizeof...(Factors)>& values,
                                         std::index_sequence<I...>) {
        return storage_type{Factors::Element::from_integer(values[I])...};
    }

    static storage_type from_coordinates_storage(std::initializer_list<std::int64_t> values) {
        if (values.size() != sizeof...(Factors)) {
            throw std::invalid_argument("Incorrect number of direct sum coordinates");
        }
        std::array<std::int64_t, sizeof...(Factors)> coordinates{};
        std::size_t position = 0;
        for (auto value : values) {
            coordinates[position++] = value;
        }
        return coordinates_impl(coordinates, std::index_sequence_for<Factors...>{});
    }

    static std::uint64_t cardinality() {
        std::uint64_t result = 1;
        auto accumulate = [&](std::uint64_t factor) {
            if (factor == 0 || result > std::numeric_limits<std::uint64_t>::max() / factor) {
                throw std::overflow_error("Direct sum cardinality exceeds the supported range");
            }
            result *= factor;
        };
        (accumulate(Factors::cardinality()), ...);
        return result;
    }
};

}
