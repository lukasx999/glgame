#include <print>
#include <cassert>

#include <gfx.hh>

using gfx::Vec;

int main() {

    Vec v1(1, 1);
    Vec v2(1, 1);
    assert(v1 - v2 == Vec(0, 0));
    assert(v1 + v2 == Vec(2, 2));
    assert(v1 * v2 == Vec(1, 1));
    assert(v1 / v2 == Vec(1, 1));
    // TODO:
    // assert(v1.rotated(gfx::Degrees(90)) == Vec(0, 1));

}
