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

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;
        Ok("".to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        let mut ret = 0;
        Ok("".to_string())
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
