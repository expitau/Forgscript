# Forgscript

### About
Forglang is an esoteric programming language (similar to [brainfuck](https://en.wikipedia.org/wiki/Brainfuck)), whose distinguishing feature is non-linear code execution.

## Documentation
A forgscript is a 2d grid of characters. Line numbering starts at 1 at the top and increases down. Column numbering starts at 1 and increases to the right. For example, the following a forgscript program.
```
...v
.v..
v...
```
The number of rows is defined by the lines in the script, but the number of columns is infinite. The forg will continue jumping even if he is out of bounds on the x-axis.

Control flow begins at cell 1,1 (the upper left corner). Every iteration of the program, the forg will jump to a new cell. The rules for where the forg will jump are as follows

- The forg's `x` coordinate will change according to the Collatz process, with one modification (this will be explained in more detail later):
  - If the forg's `x` coordinate is odd, he will jump to `3x + 1`
  - If the forg's `x` coordinate is even, he will jump to `x/2` **unless** the symbol he is currently on is a `*` **and** the register corresponding to his column is `0`
- The forg's `y` coordinate will remain the same, unless:
  - The symbol the forg is on is a `^`, in which case the forg will jump to `y - 1`
  - The symbol the forg is on is a `v`, in which case the forg will jump to `y + 1`
- If the forg's `y` coordinate is out of bounds, the program will terminate. The board extends infinitely on the `x` axis, and so the forg cannot go out of bounds horizontally.

Each column of the program has a single cell of memory. This is a 32-bit integer that can be negative. The forg will add and subtract numbers from the **column** he is in, regardless of what row he is on. If the symbol the forg is on is a `+`, he will add one to the cell. If it is a `-`, he will subtract one. If it is a `<`, he will store an input from stdin to the cell (overwriting what was there). If it is a `>`, he will write the cell to stdout.

## Commands

Symbol|Action
-|-
**Control flow**
`*`| Force the forg to jump `3x + 1` **if** the memory cell is `0`. This does nothing if the current x value is odd
`^`| Move to the above row after jumping
`v`| Move to the below row after jumping
`.`| No operation, equivalent to any undefined symbol
**Memory**
`+`| Add one to the memory cell
`-`| Subtract one from the memory cell
**Input/Output**
`<`| Store stdin to the memory cell
`>`| Write the memory cell to stdout


## Examples

### Example 1 - Print '1' to stdout
```
+..v
>..v
```

### Explanation of Example 1
The forg starts at the upper left corner `(y, x) = (1, 1)` (y is the line number, so we write it first). The symbol here is a `+`, so the memory cell for column 1 is incremented. The memory for the program is now `1, 0, 0, 0, ...`

Because the x-coordinate of the forg is `1`, and `1` is odd. He will jump to `3(1) + 1 = 4`. So, the next step of the program he is at `(y, x) = (1, 4)`.

`(1, 4)` has symbol `v`, so his next jump will be to the next line. `4` is even, so we jump to `(1 + 1, 4/2)` or `(y, x) = (2, 2)`. 

`(2, 2)` has symbol `.`, which is no operation. The forg does nothing, and jumps to `(2, 1)`.

`(2, 1)` has symbol `>`, which will write the current column's memory to stdout. If you remember, the memory is `1, 0, 0, 0, ...`, and the first column is `1`, so forg writes `1` to stdout.

`(2, 4)` has symbol `v`, so the forg will try to jump to `(3, 2)`. However, `y=3` is out of bounds, so the program terminates here.

### Code
```
v*.v..<<....................................................................
^*.^...........v............................................................
.^.v...*................v...................................................
.*.^..+-...................................................................v
...v........................................................................
.*....>..............v......................................................
```

### Explanation
Read the above explanation for basic movement rules, which will be assumed in this explanation. This explanation will assume the user is adding `1 + 2`

The forg's path starts as

```(1, 1) -> (2, 4) -> (1, 2)```

On cell `(1, 2)`, the forg reads a `*`. This means he will move on the x-axis by `3x + 1` **if** the memory cell for the column is zero, and to `x/2` otherwise. Here the memory cell is `0`, so he jumps to `(1, 7)`. This is used here to break out of the `1 -> 4 -> 2 -> 1` collatz cycle and proceed to more code.

On cell `(1, 7)` an integer is read from stdin. It reads a `1` and stores it in memory for column `7`. The forg continues

```(1, 7) -> (1, 22) -> (1, 11) -> (1, 34) -> (1, 17) -> (1, 52) -> (1, 26) -> (1, 13) -> (1, 40) -> (1, 20) -> (1, 10) -> (1, 5) -> (1, 16) -> (1, 8)```

On `(1, 8)` a `2` is read from stdin and is now stored in memory for column `8`. Memory looks like this

```0, 0, 0, 0, 0, 0, 1, 2...```

The forg continues

```(1, 8) -> (1, 4) -> (2, 2) -> (2, 7) -> (2, 22) -> (2, 11) -> (2, 34) -> (2, 17) -> (2, 52) -> (2, 26) -> (2, 13) -> (2, 40) -> (2, 20) -> (2, 10) -> (2, 5) -> (2, 16) -> (3, 8)```

The symbol on `(3, 8)` is a `*`, and the memory in column `8` is currently a two. Two is not zero, so the `*` is ignored, and the forg continues normally.

```(3, 8) -> (3, 4) -> (4, 2) -> (4, 7)```

Symbol `(4, 7)` is `+`, so 1 is added to column `7`. Memory is now

```0, 0, 0, 0, 0, 0, 2, 2...```

The forg continues

```(4, 7) -> (4, 22) -> ... -> (4, 8)```

On `(4, 8)` the symbol is `-`, so 1 is removed from column `8`. Memory is now 

```0, 0, 0, 0, 0, 0, 2, 1...```

The forg continues

```(4, 8) -> (4, 4) -> (3, 2) -> (2, 1) -> (1, 4)```

Notice that we've already been here, so the forg has successfully looped. The forg will continue looping, adding 1 to column 7 and removing 1 from column 8 until column 8 is zero. At this point the forg will move from

```(3, 8) -> (3, 25) -> (4, 76) -> (5, 38) -> ... -> (5, 4) -> (6, 2) -> (6, 7)```

The value of memory cell `7` is printed. It is a '3'. The forg goes `(6, 7) -> (6, 22) -> (7, 11)` and terminates.

## Langage Support

Forglang has official VSCode Language Support available [here](language/README.md). To install copy the "forgscript/language" folder to `$HOME\.vscode\extensions`, and it should enable automatically.

## Compilation

Use `cargo build --release` to compile the rust from the source, or use the standalone executable provided ([Latest release](https://github.com/expitau-dev/Forglang/releases/latest)).

To run a forgscript program, use the
```.\forgscript.exe <path>```, passing the path to your script as an argument. ```.\forgscript.exe --help``` displays help information. 

You can pass the `--debug` flag to run the program in debug mode, or `--ascii` to use ascii as stdin/stdout instead of the default integers.
