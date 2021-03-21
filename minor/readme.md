# Minor

**Anirudha Kulkarni**
**2019CS50421**

#### Input:

MIPS instruction set with add, addi, sw and lw operations

## Approach:
consider
```bash
lw $t1, 1000
add $t2, $t1, $t2
```
In this case we have `lw` which enguages `$t1` in the `COL_ACCESS_DELAY` duration. In `ROW_ACCESS_DELAY` time we will have `ROWBUFFER` copying elements from `DRAM`. So in this duration we can use this register to perform instructions which involve access to that particular register only or any other register for both read and write operations.
But as nothing was mentioned how many cycles are needed for accessing the data from register we assume it will be only during `COL_ACCESS_DELAY` only as mentioned in assignment statement `Copy the data at the column offset from the row buffer to the REGISTER. Time for this operation: COL_ACCESS_DELAY`.
Also using the same register in access method does not change the sequential nature of the program. Hence Non-blocking access is possible.

conside
```bash
lw $t1, 1000
add $t2, $t1, $t2
```
In this case we have loading the address value to the register. Hence we can not allow parallel execution of any instruction involving that register as its value is going to be changed. Hence only instructions with other registers can be safely allowed to execute.

![](image/readme/1616320418219.png)




## Reasoning For Approach:

### 1. We need to wait if 2 memory access commands come after each other:

consider

```bash
lw $t0, 100
lw $t1, 200
```

Here after we start executing 1st line memory unit will be busy for next ROW_ACCESS_DELAY + COL_ACCESS_DELAY or 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY according to past conditions and hence can not be accessed till then.

[PowerPoint Presentation (utah.edu)](https://my.eng.utah.edu/~cs7810/pres/11-7810-12.pdf) slide 4

Hence 2nd line will be executed only after complete execution of line 1

### 2. We can not skip an instruction and execute next instruction

consider,

```bash
lw $t0, 100
lw $t1, 200
addi $t3, $t3, 10
```

There is scope for optimization by excuting line 3 after line 1 is started to execute. But as it was mentioned in [this](https://piazza.com/class/kl581hef8tz6p7?cid=56)) piazza post, it can not skip an instruction (in this case line 2) and execute line 3 as it will violet sequential condition of execution

**Hence problem reduces to find next instruction with registers only with no conflict and execute it in parallel**


### 3. Copying back buffer:

Row stored in the buffer needs to be copied back to the DRAM after last execution is over

## Testing:

1. Parallel execution possible

   ```bash
   sw $t1, 1024
   add $t3, $t3, $t2

   ```

   ![](image/readme/1616327991458.png)
2. simlutaneous memory instructions

   ```bash
   sw $t1, 1024
   lw $t3, 9999
   ``````

   ![](image/readme/1616327893774.png)
3. jump at 13th instruction

   ```bash
   sw $t1, 1024
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   add $t3, $t3, $t2
   ```

   ![](image/readme/1616328153135.png)
   
4. nsaf

   ```bash
   lw $t1, 1024
   add $t3, $t1, $t2
   add $t3, $t1, $t2
   ```

   ![](image/readme/1616328220281.png)
   
5.  No sw/lw command: 
   ```bash
   add $t3, $t1, $t2
   ```
  
   ![](image/readme/1616328311976.png)
  
6. 