use day_trait::Day;
use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result};

#[derive(Eq,Hash,PartialEq,Debug)]
struct Pos {
    y: usize,
    x: usize,
}

#[derive(Debug)]
pub struct Day03 {
    nums: Vec<ElemNum>,
    syms: HashMap<Pos, ElemSym>,
}

#[derive(Debug)]
struct ElemNum {
    num: u32,
    y: usize,
    x: std::ops::Range<usize>,
}

#[derive(Debug)]
struct ElemSym {
    sym: char,
    nums: Vec<usize>
}

impl Day03 {
    pub fn new() -> Day03 {
        Day03{
            nums: vec![],
            syms: HashMap::new(),
        }
    }
}

impl Day for Day03 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day03.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day03.dat.testing").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        for (y,line) in buf.lines().enumerate() {
            let line = line?;
            let mut start:i32 = -1;
            let mut acc = 0;
            for (x,c) in line.chars().enumerate() {
                match c {
                    '.' => {
                        if acc != 0 {
                            self.nums.push(ElemNum { num: acc, y: y, x: ((start+1) as usize)..x });
                        }
                        start = x as i32;
                        acc = 0;
                    }
                    '0'..='9' => {
                        acc = acc*10 + c.to_digit(10).context("failed to parse digit")?;
                    }
                    _ => {
                        self.syms.insert(Pos {y, x}, ElemSym{sym:c, nums:vec![]});
                        if acc != 0 {
                            self.nums.push(ElemNum { num: acc, y: y, x: ((start+1) as usize)..x });
                        }
                        start = x as i32;
                        acc = 0;
                    }
                }
            }
            if acc != 0 {
                self.nums.push(ElemNum { num: acc, y: y, x: ((start+1) as usize)..line.len() });
            }
        }
        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;

        'num: for e in &self.nums {
            let mut s = e.x.start;
            if s > 0 {
                s -= 1;
            }
            for x in s..e.x.end+1 {
                let pos = Pos{x:x, y:e.y+1};
                if self.syms.contains_key(&pos) {
                    ret += e.num;
                    continue 'num;
                }
                if e.y > 0 {
                    let pos = Pos{x:x, y:e.y-1};
                    if self.syms.contains_key(&pos) {
                        ret += e.num;
                        continue 'num;
                    }
                }
            }
            let pos = Pos{x:s, y:e.y};
            if self.syms.contains_key(&pos) {
                ret += e.num;
                continue 'num;
            }
            let pos = Pos{x:e.x.end, y:e.y};
            if self.syms.contains_key(&pos) {
                ret += e.num;
                continue 'num;
            }
        }

        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        // collect which nums are adjacent to each symbol
        for (idx,e) in self.nums.iter().enumerate() {
            let mut s = e.x.start;
            if s > 0 {
                s -= 1;
            }
            for x in s..e.x.end+1 {
                let pos = Pos{x:x, y:e.y+1};
                if let Some(s) = self.syms.get_mut(&pos) {
                    s.nums.push(idx);
                }
                if e.y > 0 {
                    let pos = Pos{x:x, y:e.y-1};
                    if let Some(s) = self.syms.get_mut(&pos) {
                        s.nums.push(idx);
                    }
                }
            }
            let pos = Pos{x:s, y:e.y};
            if let Some(s) = self.syms.get_mut(&pos) {
                s.nums.push(idx);
            }
            let pos = Pos{x:e.x.end, y:e.y};
            if let Some(s) = self.syms.get_mut(&pos) {
                s.nums.push(idx);
            }
        }

        // go over '*' and multiply and then sum up adjacent numbers
        let ret:u32 = self.syms
            .iter()
            .filter(|(_,x)| x.nums.len() == 2 && x.sym == '*')
            .map(|(_,x)| {
                x.nums
                    .iter()
                    .map(|x| {
                        self.nums[*x].num
                    })
                    .fold(1, |acc,x| acc * x)
            }).sum();

        Ok(ret.to_string())
    }
}

impl Default for Day03 {
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
