# Forglang

## Documentation
Control flow begins at the 'forg', or @ symbol. Every iteration of the program, the forg will jump to a new command given by:

 - (Current line number) / 2 if the line number is even.
  - 1 + (Current line number) * 3 if the line number is odd.

Additionally, if the forg lands in water, he will continue swimming in the direction he was travelling until he reaches another command or a duck lets him on its back. Note that in the case that the map contains no water or ducks, the forg evaluates the [Collatz Conjecture](https://en.wikipedia.org/wiki/Collatz_conjecture).

The forg remembers things using its brain, which acts like a stack. By default, every cell in the stack is initialized to zero. The `+` and `-` commands increment or decrement the value of one of the memory cells in the stack.

## Commands

### Control flow
`o` is a lillypad. The forg will sit here for a specified memory cell's period of time (none by default)

`d` a duck. Will let the forg sit on his back if the forg has a nonzero number in the memory cell specified. The duck acts as a lillypad if forg sits on his back, or as water otherwise.

`p` upside-down duck. Will let the forg sit on his back if the forg has a zero at the memory cell specified.

`~` Water, the forg will swim past here without halting.

`@` is the forg's starting location. If there are multiple `@`s in the program, all but the first fall asleep and act as lillypads. The forg always starts on a lillypad.

`z` Forg bedtime. Forg will slep and program will stop. 

### IO

`*` a bug. The forg will eat one character from stdin and set the memory cell specified

`^` The forg will ribbit one character to stdout from the memory cell specified

### Operations

`+` The forg will increment the memory cell specified.

`-` The forg will decrement the memory cell specified.





## Example

### Code
The following program will output the sum of two numbers. (line numbers included for clarity)
```
1  |  @
2  |  ~
3  |  ~
4  |  ~
5  |  *1
6  |  ~
7  |  -1
8  |  ~
9  |  p1
10 |  ~
11 |  ~
12 |  ~
13 |  ~
14 |  z
15 |  ~
16 |  ~
17 |  ~
18 |  *2
19 |  ~
20 |  ~
21 |  ~
22 |  +2
23 |  ~
24 |  ~
25 |  ~
26 |  ~ 
27 |  ~
28 |  ^2
```
Forglang ignores whitespace, so equivalent code would be
```
@~~~*1~-1~p1~~~~z~~~*2~~~+2~~~~~^2
```

### Explanation
Control flow starts at the forg on line 1, this is an odd number so the forg jumps to line 4 and then swims to line 5. `*1` reads one number into forg memory cell 1 (lets say, the number 3). Forg then jumps to line 16 and swims to line 18. `*2` reads another number into cell 2 (lets say this number is 5). 

18 is even, so the forg then hops back to line 9. Because memory cell 1 is not zero, `p1` (the upside down duck) decides to not let forg onto its back. Forg swims to line 7, and decrements memory cell 1 (now 2). Forg jumps to line 22, and increments memory cell 2 (now 6). Forg jumps back to line 11 and swims to the upside-down duck on line 9. Memory cell 1 still isn't zero, so forg continues swimming until he once again reaches line 7. 

He continues decrementing cell 1 and incrementing cell 2 until cell 1 is zero and cell 2 is eight. Now, forg swims to the upside-down duck and (because cell 1 is zero) the duck lets him on his back, so forg can jump all the way to line 28, where he outputs 8 to stdout. He finally jumps to line 14, where he falls asleep and the program terminates.

## Compilation
Run a forglang program using the syntax
```./forglang.exe <path>```, passing the path to your script as an argument.