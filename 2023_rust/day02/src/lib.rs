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

pub struct Day02 {
    games: Vec<Game>,
}

#[derive(Debug)]
struct Color(u32, u32, u32);

impl Color {
    fn power(&self) -> u32 {
        self.0 * self.1 * self.2
    }
}

#[derive(Debug)]
struct Game {
    id: u32,
    round: Vec<Color>,
}

impl Game {
    fn possible_with_total(&self, total: &Color) -> bool {
        for r in &self.round {
            if
            r.0 > total.0 ||
            r.1 > total.1 ||
            r.2 > total.2 {
                return false;
            }
        }
        return true;
    }
    fn min_possible(&self) -> Color {
        self.round.iter().fold(Color{0:u32::MIN, 1:u32::MIN, 2:u32::MIN}, |mut acc, ele| {
            acc.0 = acc.0.max(ele.0);
            acc.1 = acc.1.max(ele.1);
            acc.2 = acc.2.max(ele.2);
            acc
        })
    }
}

impl Day02 {
    pub fn new() -> Day02 {
        Day02{
            games: vec![],
            // Game{
            //     id: u32::MAX,
            //     round: vec![],
            // },
        }
    }
}

impl Day for Day02 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day02.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day02.dat.testing").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        for line in buf.lines() {
            let line = line?;
            let line:Vec<_> = line.split(":").collect();

            let colors:Result<Vec<_>> = line[1]
                .split(";")
                .map(|round| {
                    let mut c = Color{0:0, 1:0, 2:0};
                    let cs:Vec<_> = round.trim().split(",").collect();
                    for color in cs {
                        let css:Vec<_> = color.trim().split(" ").collect();
                        match css[1] {
                            "red" => c.0 = css[0].parse().unwrap_or(0),
                            "green" => c.1 = css[0].parse().unwrap_or(0),
                            "blue" => c.2 = css[0].parse().unwrap_or(0),
                            &_ => return Err(anyhow!("Invalid color {}", css[1])),
                        }
                    }
                    Ok(c)
                }).collect();

            self.games.push(Game{
                id: line[0].strip_prefix("Game ").context("Failed parsing game id")?.parse().context("Failed fetching game id")?,
                round: colors?,
            });
        }

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;
        let total = Color{0:12, 1:13, 2:14};

        for g in &self.games {
            if g.possible_with_total(&total) {
                ret += g.id;
            }
        }

        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        let mut ret = 0;

        for g in &self.games {
            ret += g.min_possible().power();
        }

        Ok(ret.to_string())
    }
}

impl Default for Day02 {
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
