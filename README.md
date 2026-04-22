# Lab-Two
Lab 2
# Assemble the ASM
nasm -f elf64 hamming_distance.asm -o hamming_distance.o

# Compile the C test harness and link with the object file
gcc -no-pie hamming_distance_test.c hamming_distance.o -o hamming_test

# Run
./hamming_test