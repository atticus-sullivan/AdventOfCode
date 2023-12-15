use clap::Parser;
use day_trait::Day;
use anyhow::{Context, Result, anyhow};

use day01;
use day02;
use day03;
use day04;
use day05;
use day06;
use day07;
use day08;
use day09;
use day10;

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Cli {
    /// Day which to run. Defaults to all days.
    #[arg(short, long)]
    day: Option<usize>,

    /// Part which to run. Defaults to both parts.
    #[arg(short, long)]
    part: Option<usize>,
}

fn main() -> Result<()> {
    let mut days: [Box<dyn Day>; 10] = [
        Box::new(day01::Day01::new()),
        Box::new(day02::Day02::new()),
        Box::new(day03::Day03::new()),
        Box::new(day04::Day04::new()),
        Box::new(day05::Day05::new()),
        Box::new(day06::Day06::new()),
        Box::new(day07::Day07::new()),
        Box::new(day08::Day08::new()),
        Box::new(day09::Day09::new()),
        Box::new(day10::Day10::new()),
    ];

    let args = Cli::parse();

    match args.day {
        Some(d) => {
            let x = days.get_mut(d-1).context("invalid day provided")?;
            x.parse()?;
            match args.part {
                Some(1) => {
                    println!("Day {:02} Part 1: {}", d, x.solve_part1()?);
                },
                Some(2) => {
                    println!("Day {:02} Part 2: {}", d, x.solve_part2()?);
                },
                Some(_) => {
                    return Err(anyhow!("invalid part provided"));
                }
                None => {
                    println!("Day {:02} Part 1: {}", d, x.solve_part1()?);
                    println!("Day {:02} Part 2: {}", d, x.solve_part2()?);
                },
            }
        },
        None => {
            let mut first = true;
            for (day,x) in days.iter_mut().enumerate() {
                if !first {
                    println!("");
                } else {
                    first = false;
                }
                x.parse()?;
                match args.part {
                    Some(1) => {
                        println!("Day {:02} Part 1: {}", day+1, x.solve_part1()?);
                    },
                    Some(2) => {
                        println!("Day {:02} Part 2: {}", day+1, x.solve_part2()?);
                    },
                    Some(_) => {
                        return Err(anyhow!("invalid part provided"));
                    }
                    None => {
                        println!("Day {:02} Part 1: {}", day+1, x.solve_part1()?);
                        println!("Day {:02} Part 2: {}", day+1, x.solve_part2()?);
                    },
                }
            }
        },
    }

    return Ok(());
}
