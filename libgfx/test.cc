#include <cassert>

#include <gfx.hh>

int main() {
    gfx::Color color(0xff000000);
    assert(color.r == 0xff);
    assert(color.g == 0);
    assert(color.b == 0);
    assert(color.a == 0);

    assert(color.normalized().r == 1);

}
