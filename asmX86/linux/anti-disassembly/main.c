#include <assert.h>

int junk_bytes();
int overlapping_instructions();
int call_ret_abuse();

int main() {
    assert(junk_bytes() == 1);
    assert(overlapping_instructions() == 1);
    assert(call_ret_abuse() == 1);
}
