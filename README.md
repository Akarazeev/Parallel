File `generator.c` generates Matrix (size\*size):
1. `gcc generator.c -o generator.out`
2. `./generator.out <size>`
3. And there is "matrix.bin" file with Matrix (size\*size) inside it.

Then it is necessary to run `main.c`:
1. `gcc -pthread main.c -o main.out`
2. `./main.out <num_threads>`
3. Output contains:
    ```
    # MAXIMUM
    # indexes of MAXIMUM
    # MINIMUM
    #indexes of MINIMUM
    ```
