use crossterm::{cursor, terminal, ExecutableCommand, QueueableCommand};
use std::env;
use std::fs;
use std::io;
use std::io::{stdout, Write};

fn render_registers(registers: &Vec<i32>) {
    fn get_digit(i: i32, d: u32) -> i32 {
        let digits = i.to_string().len() as u32;
        return if digits > d {
            i / (10 as i32).pow(d) % 10
        } else {
            -1
        };
    }
    let max = *registers.iter().max().unwrap();
    let max_digits = max.to_string().len();
    for d in (0..max_digits).rev() {
        for r in registers.iter() {
            let digit = get_digit(*r, d as u32);
            if digit == -1 {
                print!(" ")
            } else {
                print!("{}", digit)
            }
        }
        println!();
    }
}

fn render_grid(grid: &Vec<Vec<char>>, x: i32, y: i32) {
    for (i, row) in grid.iter().enumerate() {
        for (j, cell) in row.iter().enumerate() {
            print!(
                "{}",
                if (i, j) == (y as usize, x as usize) {
                    'o'
                } else {
                    match cell {
                        '.' => ' ',
                        _ => *cell,
                    }
                }
            )
        }
        println!();
    }
}

fn load_script(path: &String) -> String {
    return fs::read_to_string(path).expect("Something went wrong reading the file");
}

fn get_grid(script: &String) -> Vec<Vec<char>> {
    let rows = script.split("\r\n").collect::<Vec<&str>>();
    let mut grid: Vec<Vec<char>> = Vec::new();

    for row in rows.iter() {
        let mut g_row = Vec::new();
        for c in row.chars() {
            g_row.push(c);
        }
        grid.push(g_row);
    }

    return grid;
}

fn div2(x: i32) -> i32 {
    (x + 1) / 2 - 1
}

fn mult3add1(x: i32) -> i32 {
    (x + 1) * 3
}

fn collatz(x: i32) -> i32 {
    if (x + 1) % 2 == 0 {
        div2(x)
    } else {
        mult3add1(x)
    }
}

fn input(ascii_mode: bool) -> i32 {
    loop {
        let mut input_text = String::new();
        print!("> ");
        io::stdout().flush().unwrap();
        io::stdin()
            .read_line(&mut input_text)
            .expect("failed to read from stdin");
        let trimmed = input_text.trim();
        if !ascii_mode {
            if let Ok(i) = trimmed.parse::<i32>() {
                return i;
            } else {
                println!("this was not an integer: '{}'", trimmed)
            }
        } else {
            if let Ok(i) = trimmed.parse::<char>() {
                return i as i32;
            } else {
                println!("this was not an char: '{}'", trimmed)
            }
        }
    }
}

fn output(data: i32, ascii_mode: bool) {
    if !ascii_mode {
        println!("Forg says {}", data);
    } else {
        println!("Forg says {}", char::from_u32(data as u32).unwrap_or('�'))
    }
}

fn get_next(x: i32, y: i32, cell: &char, val: i32, ascii_mode: bool) -> (i32, i32, i32) {
    (
        match cell {
            '*' => {
                if val == 0 {
                    mult3add1(x)
                } else {
                    collatz(x)
                }
            }
            _ => collatz(x),
        },
        match cell {
            'v' => y + 1,
            '^' => y - 1,
            _ => y,
        },
        match cell {
            '+' => val + 1,
            '-' => val - 1,
            '<' => input(ascii_mode),
            '>' => {
                output(val, ascii_mode);
                val
            }
            _ => val,
        },
    )
}

fn clear() {
    let mut stdout = stdout();

    stdout
        .execute(terminal::Clear(terminal::ClearType::FromCursorUp))
        .unwrap();
    stdout.queue(cursor::MoveTo(0, 0)).unwrap();
}

fn help(){
    println!("Usage: forgscript <path> [options]");
    println!("    path     The path to a forgscript file");
    println!("    options  The options to run this script with");
    println!("        --debug  Run in debug mode");
    println!("        --ascii  Use ascii inputs and outputs instead of integers");
}

fn main() {
    let command: Vec<String> = env::args().collect();
    let (_, args) = command.split_at(1);

    if args.contains(&"--help".to_string()) || args.len() == 0 {
        help();
        return;
    }

    let filepath = args.get(0).unwrap();

    let ascii_mode = args.contains(&"--ascii".to_string());
    let debug_mode = args.contains(&"--debug".to_string());

    let script = load_script(filepath);

    let mut fx = 0;
    let mut fy = 0;

    let grid = get_grid(&script);
    let mut registers: Vec<i32> = vec![0; grid.iter().map(|x| x.clone().len()).max().unwrap_or(0)];

    let mut steps = 0;

    for i in 0.. {
        steps = i;

        if fy as usize >= grid.len()
            || (fy as usize == grid.len() - 1 && grid[fy as usize].len() == 0)
        {
            break;
        }

        let cell = grid
            .get(fy as usize)
            .unwrap()
            .get(fx as usize)
            .unwrap_or(&'.');

        if debug_mode {
            clear();
            println!("{: >3} | y: {}  x: {}  c: {}\n", steps, fy + 1, fx + 1, cell);
            render_registers(&registers);
            render_grid(&grid, fx, fy);
        }
        let next = get_next(
            fx,
            fy,
            cell,
            *registers.get(fx as usize).unwrap_or(&0),
            ascii_mode,
        );
        if let Some(_) = registers.get(fx as usize) {
            registers[fx as usize] = next.2;
        }
        fx = next.0;
        fy = next.1;

        if debug_mode {
            print!("Press enter to continue ");
            io::stdout().flush().unwrap();

            let mut _input = String::new();
            io::stdin()
                .read_line(&mut _input)
                .expect("failed to read from stdin");
        }
    }
    println!();
    println!("Finished in {} steps", steps);
}
