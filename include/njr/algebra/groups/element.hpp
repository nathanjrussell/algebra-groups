#pragma once

#include <concepts>
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <njr/algebra/core/core.hpp>

namespace njr::algebra {

template<class Model, class Parent>
class AdditiveElement : public AdditiveOperators<AdditiveElement<Model, Parent>> {
public:
    using parent_type = Parent;
    using algebra = typename Model::descriptor;
    using storage_type = typename Model::storage_type;

private:
    storage_type value_ = Model::zero_storage();

public:
    AdditiveElement() = default;

    static AdditiveElement from_storage(storage_type value) {
        AdditiveElement result;
        result.value_ = Model::normalize_storage(std::move(value));
        return result;
    }

    static AdditiveElement from_integer(std::int64_t value)
        requires requires { Model::from_integer_storage(value); }
    {
        return from_storage(Model::from_integer_storage(value));
    }

    static AdditiveElement zero() {
        return AdditiveElement{};
    }

    static AdditiveElement generator()
        requires requires { Model::generator_storage(); }
    {
        return from_storage(Model::generator_storage());
    }

    AdditiveElement add(const AdditiveElement& other) const {
        return from_storage(Model::add_storage(value_, other.value_));
    }

    AdditiveElement negate() const {
        return from_storage(Model::negate_storage(value_));
    }

    bool is_zero() const {
        return value_ == Model::zero_storage();
    }

    const storage_type& storage() const {
        return value_;
    }

    storage_type representative() const requires std::integral<storage_type> {
        return value_;
    }

    const parent_type& parent() const {
        static const parent_type instance;
        return instance;
    }

    bool operator==(const AdditiveElement& other) const {
        return value_ == other.value_;
    }
};

template<class Model>
concept AdditiveStorageModel = requires(const typename Model::storage_type& a,
                                        const typename Model::storage_type& b) {
    typename Model::descriptor;
    requires std::derived_from<typename Model::descriptor::additive_category, AbelianGroupCategory>;
    { Model::zero_storage() } -> std::same_as<typename Model::storage_type>;
    { Model::add_storage(a, b) } -> std::same_as<typename Model::storage_type>;
    { Model::negate_storage(a) } -> std::same_as<typename Model::storage_type>;
};

template<AdditiveStorageModel Model>
class AbelianGroup : public AdditiveGroupStructure<AdditiveElement<Model, AbelianGroup<Model>>> {
public:
    using Element = AdditiveElement<Model, AbelianGroup<Model>>;
    using storage_type = typename Model::storage_type;

    static Element element(storage_type value) {
        return Element::from_storage(std::move(value));
    }

    static Element element(std::initializer_list<std::int64_t> coordinates)
        requires requires { Model::from_coordinates_storage(coordinates); }
    {
        return element(Model::from_coordinates_storage(coordinates));
    }

    static std::uint64_t cardinality() {
        return Model::cardinality();
    }
};

}
