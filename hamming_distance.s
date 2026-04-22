/* =============================================================================
 * hamming_distance.s  –  CMPE 310 Project 2
 *
 * Computes the Hamming distance between two null-terminated ASCII strings.
 * Hamming distance = number of bit positions that differ across the ASCII
 * binary representations of the two strings.
 * If the strings differ in length, only the shorter length is compared.
 *
 * Prototype (C-callable, System V AMD64 ABI):
 *   int hamming_distance(const char *s1, const char *s2);
 *
 * Arguments:
 *   %rdi  -> s1  (first  string, null-terminated)
 *   %rsi  -> s2  (second string, null-terminated)
 *
 * Returns:
 *   %eax  <- Hamming distance (32-bit integer)
 *
 * Registers used:
 *   %rdi, %rsi  – string pointers (advanced each iteration)
 *   %cl         – current byte from s1
 *   %dl         – current byte from s2
 *   %ecx        – XOR result / bit-counting scratch
 *   %edx        – lowest-bit scratch during popcount
 *   %eax        – running total (return value)
 * =============================================================================
 */

        .section .note.GNU-stack,"",@progbits   /* mark stack non-executable */

        .text
        .globl  hamming_distance
        .type   hamming_distance, @function

hamming_distance:
        xorl    %eax, %eax              /* eax = total distance, start at 0  */

.loop:
        movzbl  (%rdi), %ecx            /* ecx = (unsigned) *s1              */
        movzbl  (%rsi), %edx            /* edx = (unsigned) *s2              */

        testl   %ecx, %ecx
        jz      .done                   /* s1 hit NUL → stop                 */
        testl   %edx, %edx
        jz      .done                   /* s2 hit NUL → stop                 */

        xorl    %edx, %ecx              /* ecx = s1[i] XOR s2[i]             */
                                        /* 1-bits mark differing positions    */

        /* --- popcount: count set bits in %ecx via shift-and-test loop --- */
.popcount:
        testl   %ecx, %ecx
        jz      .next                   /* no bits left → next character      */

        movl    %ecx, %edx
        andl    $1,   %edx              /* isolate lowest bit                 */
        addl    %edx, %eax              /* add to running total               */
        shrl    $1,   %ecx             /* unsigned shift right               */
        jmp     .popcount

.next:
        incq    %rdi                    /* advance both string pointers       */
        incq    %rsi
        jmp     .loop

.done:
        ret                             /* %eax holds the Hamming distance    */

        .size   hamming_distance, .-hamming_distance