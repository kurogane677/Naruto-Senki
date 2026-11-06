// Unit tests for game logic that runs without a scene (doctest).
// Build with cmake -D_AX_TESTS=ON and run NarutoSenki_test.
#include "doctest.h"

// the engine's doctest library holds the implementation; only main is ours
int main(int argc, char** argv)
{
    doctest::Context context(argc, argv);
    return context.run();
}
