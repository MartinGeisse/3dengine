
#include "../src/Fixed.h"

static bool anyFailed = false;

void assert(bool condition, const char *message) {
    printf("%s: %s\n", condition ? "PASSED" : "FAILED", message);
    if (!condition) {
        anyFailed = true;
    }
}

int main(void) {
    assert(5 == 5, "foo");
    // assert(5 == 6, "bar");
    if (anyFailed) {
        printf("\n*** TEST FAILED ***\n\n");
    }
    return 0;
}
