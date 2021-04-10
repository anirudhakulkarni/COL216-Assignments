# Assignment - 4

**Anirudha Kulkarni :** **2019CS50421**

**Pratyush Saini :** **2019CS10444**

#### Input:

MIPS instruction set with add, addi, sw and lw operations

### Build Instructions:

Program can be run easily with make instructions:

1. To Build: `make build2`
2. To run executable from build `make run2`
3. To remove build file `make clean2`

All these steps can be performed by `make nba` which will remove previous build, create new one and give output.

Custom `ROW_ACCESS_DELAY` and `COL_ACCESS_DELAY` can be provided by navigating to `build` directory and

```bash
./main ../input.txt 100 45
```

which will execute the program with `ROW_ACCESS_DELAY` as 100 and `COL_ACCESS_DELAY` as 45
Provide input in input.txt or give it as command line argument.
Defaults used when `make all` is used are:

```bash
`ROW_ACCESS_DELAY`: 10
`COL_ACCESS_DELAY`: 2
input location : 'input.txt' 
```

Code with DRAM Reordering can be run by:

```bash
make dram
```

## Approach:

### 1. DRAM Reordering:

### 2. DRAM Reordering with NBA:

## Assumptions:

1. Registers are accessed only during `COL_ACCESS_DELAY` duration
2. `ROW_ACCESS_DELAY` > `COL_ACCESS_DELAY` and both are non zero
3. Instructions dont consume cycle to be accessed from memory
4. Maximum instructions and data values are 2** 19 so that overall memory will not exceed 2 ** 20

## Strengths:

1. Maintains the sequential nature of the program rather than manipulating it via lookaheads.
2. Reduced complexity compared with priority queue which requires checking in distinct future values which can invlove lookaheadd of `O(N)` plus backtracking of `O(N)` and hence increase evaluation time in case of larger inputs. Ex: `10 sec` vs `30 sec` for program with `10**10` lines which is order of a general MIPS processor handles.
3. Modular and easily understandable approach due to Object oriented approach
4. Debugging friendly approach which makes easier to follow the execution of program as lookahead is `O(ROW_ACCESS_DELAY)`

## Weakness:

1. In contrast to approach with queue for prioritising request this will not be able to optimise the cycles in the distinct future as notion of lookahead and backtracking was avoided.
   This can be severe in case of specific operations like `lw $t1, 1000` and high values of `ROW_ACCESS_DELAY`. In such cases we will waste lot of cycles which could have been saved in the future.
   But such instructions are very specific in occurence.
2. Object oriented model might take large space even for smaller inputs. All the variables are needed to be initialised and take constant space in contrast to functional approach which creates variables as we need it but looses modularity and order of computations in a MIPS processor is much larger than such case.
