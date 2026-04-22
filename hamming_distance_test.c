/*
 * hamming_distance_test.c  –  CMPE 310 Project 2
 *
 * C test harness for the assembly implementation of hamming_distance().
 * The actual computation is done entirely in hamming_distance.asm.
 *
 * Build:
 *   nasm -f elf64 hamming_distance.asm -o hamming_distance.o
 *   gcc -no-pie hamming_distance_test.c hamming_distance.o -o hamming_test
 *   ./hamming_test
 */

#include <stdio.h>
#include <string.h>

/* Declared here; defined in hamming_distance.asm */
extern int hamming_distance(const char *s1, const char *s2);

/* ── test infrastructure ──────────────────────────────────────────────── */

typedef struct {
    const char *s1;
    const char *s2;
    int         expected;
    const char *description;
} TestCase;

static int run_tests(const TestCase *tests, int n) {
    int passed = 0;
    printf("=== Running %d test cases ===\n\n", n);

    for (int i = 0; i < n; i++) {
        int got = hamming_distance(tests[i].s1, tests[i].s2);
        int ok  = (got == tests[i].expected);
        passed += ok;

        printf("[%s] %s\n", ok ? "PASS" : "FAIL", tests[i].description);
        if (!ok) {
            printf("       s1=\"%s\"  s2=\"%s\"\n", tests[i].s1, tests[i].s2);
            printf("       expected=%d  got=%d\n", tests[i].expected, got);
        }
    }

    printf("\n=== %d / %d passed ===\n\n", passed, n);
    return passed == n;
}

/* ── test cases ───────────────────────────────────────────────────────── */

int main(void) {

    /* --- Lab-provided test cases --- */
    TestCase spec_tests[] = {
        {
            "foo", "bar", 8,
            "Lab spec: \"foo\" vs \"bar\" → 8"
        },
        {
            "this is a test", "of the emergency broadcast", 38,
            "Lab spec: \"this is a test\" vs \"of the emergency broadcast\" → 38"
        },
    };

    /* --- Custom test cases --- */
    TestCase custom_tests[] = {
        {
            "", "", 0,
            "Both strings empty → 0"
        },
        {
            "a", "a", 0,
            "Identical single character → 0"
        },
        {
            "a", "b", 2,
            "'a'(0x61) XOR 'b'(0x62) = 0x03 → 2 differing bits"
        },
        {
            "ABC", "abc", 3,
            "Each uppercase/lowercase pair differs by exactly bit 5 → 3"
        },
        {
            "abc", "ab", 0,
            "s1 longer than s2: only compare up to len(s2)=2, no differences → 0"
        },
        {
            "ab", "abc", 0,
            "s2 longer than s1: only compare up to len(s1)=2, no differences → 0"
        },
        {
            "\x0f", "\xf0", 8,
            "0x0F XOR 0xF0 = 0xFF → all 8 bits differ → 8"
        },
        {
            "Hello, World!", "Hello, World!", 0,
            "Long identical strings → 0"
        },
        {
            "10101010", "01010101", 8,
            "Every character differs → 8 (one bit per char)"
        },
        {
            "AAAA", "BBBB", 8,
            "'A'(0x41) XOR 'B'(0x42) = 0x03 → 2 bits × 4 chars = 8"
        },
        {
            "abcdefghij", "ABCDEFGHIJ", 10,
            "10 chars, each differs by bit 5 only → 10 bits total"
        },
    };

    int spec_n   = sizeof(spec_tests)   / sizeof(spec_tests[0]);
    int custom_n = sizeof(custom_tests) / sizeof(custom_tests[0]);


    printf("Hamming Distance Tests\n");


    printf("── Lab-Specified Test Cases ─\n");
    int ok1 = run_tests(spec_tests, spec_n);

    printf("── Custom Test Cases ─\n");
    int ok2 = run_tests(custom_tests, custom_n);

    if (ok1 && ok2)
        printf(" All tests passed.\n");
    else
        printf(" Some tests FAILED — check assembly logic.\n");

    return (ok1 && ok2) ? 0 : 1;
}