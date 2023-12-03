use day_trait::Day;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result, anyhow};

pub struct Day<TODO> {
}

impl Day<TODO> {
    pub fn new() -> Day<TODO> {
        Day<TODO>{
        }
    }
}

impl Day for Day<TODO> {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day<TODO>.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day<TODO>.dat.testing").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        Ok(())
    }

    fn solve_part1(&mut self) -> Result<()> {
        let mut ret = 0;
        println!("Day <TODO>, Part 1: {ret}");
        Ok(())
    }

    fn solve_part2(&mut self) -> Result<()> {
        let mut ret = 0;
        println!("Day <TODO>, Part 2: {ret}");
        Ok(())
    }
}

impl Default for Day<TODO> {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
    }
}
