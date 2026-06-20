#include <cstdint>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <gtest/gtest.h>
#include <njr/algebra/groups/groups.hpp>

using namespace njr::algebra;

template<class A, class B>
concept CanAdd = requires(const A& a, const B& b) { a + b; };

using Product = AbelianGroup<DirectSum<C<2>, C<3>>>;
static_assert(CyclicGroupElement<C<5>::Element, Addition>);
static_assert(AbelianGroupElement<Product::Element, Addition>);
static_assert(!RingElement<C<5>::Element>);
static_assert(!CanAdd<C<6>::Element, Product::Element>);

TEST(CyclicGroups, NormalizesSignedRepresentatives) {
    EXPECT_EQ(C<11>::element(-1).representative(), 10u);
    EXPECT_EQ(C<11>::element(std::numeric_limits<std::int64_t>::min()).representative(), 3u);
    EXPECT_EQ(C<11>::element(25).representative(), 3u);
}

TEST(CyclicGroups, SatisfiesAllGroupLaws) {
    using G = C<5>;
    for (int a = 0; a < 5; ++a) {
        const auto x = G::element(a);
        EXPECT_TRUE((identity_law<G::Element, Addition>(x)));
        EXPECT_TRUE(x.add(x.negate()).is_zero());
        for (int b = 0; b < 5; ++b) {
            const auto y = G::element(b);
            EXPECT_EQ(x + y, y + x);
            for (int c = 0; c < 5; ++c) {
                EXPECT_TRUE((associative_law<G::Element, Addition>(x, y, G::element(c))));
            }
        }
    }
}

TEST(CyclicGroups, SupportsSignedRepetitionAndVirtualViews) {
    C<7> group;
    const interfaces::CyclicGroup<C<7>::Element, Addition>& view = group;
    const auto generator = view.generator();
    EXPECT_EQ(repeat_signed<Addition>(generator, -2), C<7>::element(5));
    EXPECT_EQ(view.inverse(Addition{}, generator), C<7>::element(6));
}

TEST(DirectSums, UsesComponentOperations) {
    const auto a = Product::element({1, 2});
    const auto b = Product::element({1, 1});
    EXPECT_TRUE((a + b).is_zero());
    EXPECT_EQ(Product::cardinality(), 6u);
    EXPECT_THROW(Product::element({1}), std::invalid_argument);
    Product group;
    EXPECT_TRUE(group.contains(a));
    EXPECT_EQ(group.combine(Addition{}, a, b), Product::Element::zero());
}

TEST(DirectSums, HandlesTrivialAndEmptyProducts) {
    EXPECT_TRUE(C<1>::element(19).is_zero());
    EXPECT_TRUE(C<1>{}.generator().is_zero());
    using Empty = AbelianGroup<DirectSum<>>;
    EXPECT_EQ(Empty::cardinality(), 1u);
    EXPECT_TRUE(Empty::Element::zero().is_zero());
}

TEST(DirectSums, ChecksCardinalityOverflow) {
    using Huge = AbelianGroup<DirectSum<C<4294967295u>, C<4294967295u>, C<2>>>;
    EXPECT_THROW(Huge::cardinality(), std::overflow_error);
}

TEST(Isomorphisms, PreservesAdditionAndRoundTrips) {
    using Map = ChineseRemainderIsomorphism<2, 3>;
    for (int a = 0; a < 6; ++a) {
        const auto x = Map::Source::element(a);
        EXPECT_EQ(Map::backward(Map::forward(x)), x);
        for (int b = 0; b < 6; ++b) {
            const auto y = Map::Source::element(b);
            EXPECT_EQ(Map::forward(x + y), Map::forward(x) + Map::forward(y));
        }
    }
}
