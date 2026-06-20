#include <iostream>
#include <tuple>
#include <njr/algebra/groups/groups.hpp>

using namespace njr::algebra;

int main() {
    using G = AbelianGroup<DirectSum<C<2>, C<3>>>;
    const auto a = G::element({1, 2});
    const auto b = G::element({1, 1});
    const auto sum = a.add(b);
    std::cout << std::get<0>(sum.storage()).representative() << ' '
              << std::get<1>(sum.storage()).representative() << '\n';
}
