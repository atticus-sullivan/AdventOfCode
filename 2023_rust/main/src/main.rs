use clap::Parser;
use day_trait::Day;
use anyhow::{Context, Result, anyhow};

use day01;
use day02;

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
    let mut days: [Box<dyn Day>; 2] = [
        Box::new(day01::Day01::new()),
        Box::new(day02::Day02::new()),
    ];

    let args = Cli::parse();

    match args.day {
        Some(d) => {
            let x = days.get_mut(d-1).context("invalid day provided")?;
            x.parse()?;
            match args.part {
                Some(1) => {
                    x.solve_part1()?;
                },
                Some(2) => {
                    x.solve_part2()?;
                },
                Some(_) => {
                    return Err(anyhow!("invalid part provided"));
                }
                None => {
                    x.solve_part1()?;
                    x.solve_part2()?;
                },
            }
        },
        None => {
            let mut first = true;
            for x in days.iter_mut() {
                if !first {
                    println!("");
                } else {
                    first = false;
                }
                x.parse()?;
                match args.part {
                    Some(1) => {
                        x.solve_part1()?;
                    },
                    Some(2) => {
                        x.solve_part2()?;
                    },
                    Some(_) => {
                        return Err(anyhow!("invalid part provided"));
                    }
                    None => {
                        x.solve_part1()?;
                        x.solve_part2()?;
                    },
                }
            }
        },
    }

    return Ok(());
}
