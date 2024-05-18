/*
Advent Of Code 2023
Copyright (C) 2024  Lukas Heindl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

use day_trait::Day;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result, anyhow};

#[derive(Clone,Debug)]
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
