# Minor

Anirudha Kulkarni

2019CS50421

## Reasoning For Approach:

### 1. We need to wait if 2 memory access commands come after each other:

consider

```bash
lw $t0, 100
lw $t1, 200
```

Here after we start executing 1st line memory unit will be busy for next ROW_ACCESS_DELAY + COL_ACCESS_DELAY or 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY according to past conditions and hence can not be accessed till then.

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

[PowerPoint Presentation (utah.edu)](https://my.eng.utah.edu/~cs7810/pres/11-7810-12.pdf) slide 4

DRAM memory access one per cycle

* register will be busy for all time. Else it will not be sequential

when to Non-blocking access:

* when next register is not free
