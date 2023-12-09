use day_trait::Day;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result};

#[derive(Clone,Debug)]
pub struct Day09 {
    values: Vec<Vec<i32>>,
    interm: Option<Vec<Vec<Vec<i32>>>>,
}

impl Day09 {
    pub fn new() -> Day09 {
        Day09{
            values: vec![],
            interm: None,
        }
    }

    fn fill(&mut self) -> Option<&Vec<Vec<Vec<i32>>>> {
        match self.interm {
            Some(_) => self.interm.as_ref(),
            None => {
                let mut res = vec![];
                for vs in &self.values {
                    let mut interm = vec![vs.clone()];
                    while !interm.last()?.iter().all(|x| x == &0) {
                        let cur = interm.last()?;
                        let mut next = vec![];
                        // build next level
                        for idx in 1..cur.len() {
                            next.push(cur[idx] - cur[idx-1]);
                        }
                        interm.push(next);
                    }
                    res.push(interm);
                }
                self.interm = Some(res);
                self.interm.as_ref()
            }
        }
    }
}

impl Day for Day09 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day09.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day09.dat.testing").context("reading input file failed")?;
        let buf = io::BufReader::new(file);
        for line in buf.lines() {
            let line = line?;
            self.values.push(line.split(" ").map(|x| x.parse().context("failed parsing a value")).collect::<Result<_>>()?);
        }

        // println!("{self:#?}");

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;

        for interm in self.fill().context("fill failed")? {
            // println!("{interm:?}");
            let mut next = 0;
            for idx in (0..interm.len()-1).rev() {
                next = interm[idx].last().context("interm vector shouldn't be empty when going up")? + next;
                // println!("{next} <- {}", interm[idx].last().context("interm vector shouldn't be empty when going up")?);
            }
            ret += next;
            // println!("{next}");
        }
        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        let mut ret = 0;

        for interm in self.fill().context("fill failed")? {
            // println!("{interm:?}");
            let mut next = 0;
            for idx in (0..interm.len()-1).rev() {
                next = interm[idx].first().context("interm vector shouldn't be empty when going up")? - next;
                // println!("{next} <- {}", interm[idx].first().context("interm vector shouldn't be empty when going up")?);
            }
            ret += next;
            // println!("{next}");
        }
        Ok(ret.to_string())
    }
}

impl Default for Day09 {
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
