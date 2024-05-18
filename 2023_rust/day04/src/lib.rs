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
use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result};

#[derive(Debug)]
struct Card {
    // id: usize,
    have: Vec<u32>,
    winning: HashSet<u32>,
    amount: usize,
}

#[derive(Debug)]
pub struct Day04 {
    cards: Vec<Card>,
}

impl Day04 {
    pub fn new() -> Day04 {
        Day04{
            cards: vec![],
        }
    }
}

impl Day for Day04 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day04.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day04.dat.testing").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        for line in buf.lines() {
            let line = line?;
            let line:Vec<_> = line.split(":").collect();
            let nums:Vec<_> = line[1].split(" | ").collect();

            self.cards.push(Card{
                // id: line[0].strip_prefix("Card ").context("Failed parsing card id")?.trim().parse().context("Failed fetching card id")?,
                have: nums[0].trim().split(" ").filter(|x| !x.is_empty()).map(|x| x.parse().context("Failed to parse number")).collect::<Result<_>>()?,
                winning: nums[1].trim().split(" ").filter(|x| !x.is_empty()).map(|x| x.parse().context("Failed to parse number")).collect::<Result<_>>()?,
                amount: 1,
            });
        }

        // println!("{self:#?}");

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;

        for c in &self.cards {
            let mut cur = 0;
            for n in &c.have {
                if cur == 0 && c.winning.contains(n) {
                    cur = 1;
                } else if cur != 0 && c.winning.contains(n) {
                    cur *= 2;
                }
            }
            ret += cur;
        }

        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        for idx in 0..self.cards.len() {
            let mut matching = 0;
            let c = &self.cards[idx];
            for n in &c.have {
                if c.winning.contains(n) {
                    matching += 1;
                }
            }
            let a = c.amount;
            for i in 1..=matching {
                self.cards[idx + i].amount += a;
            }
        }

        Ok(self.cards.iter().map(|x| x.amount).sum::<usize>().to_string())
    }
}

impl Default for Day04 {
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
