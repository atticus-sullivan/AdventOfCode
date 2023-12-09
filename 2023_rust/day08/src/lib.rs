use day_trait::Day;
use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result, anyhow};
use std::convert::TryFrom;

#[derive(Clone,Debug,Copy)]
enum Dir {
    Right,
    Left,
}

impl TryFrom<char> for Dir {
    type Error = anyhow::Error;
    fn try_from(value:char) -> Result<Self, Self::Error> {
        match value {
            'L' => Ok(Dir::Left),
            'R' => Ok(Dir::Right),
            _ => Err(anyhow!("Invalid char {value}")),
        }

    }
}

#[derive(Clone,Debug)]
pub struct Day08 {
    dirs: Vec<Dir>,
    graph: HashMap<String,(String,String,bool)>,
}

impl Day08 {
    pub fn new() -> Day08 {
        Day08{
            dirs: vec![],
            graph: HashMap::new(),
        }
    }

    fn follow(&self, src: &str, is_dst: fn(&str) -> bool) -> Result<usize> {
        let mut cur = src;
        let mut i = 0;
        let mut cnt = 0;
        while !is_dst(cur) {
            match self.dirs[i] {
                Dir::Left  => cur = &(self.graph.get(cur).context("cur is in an invalid state")?.0),
                Dir::Right => cur = &(self.graph.get(cur).context("cur is in an invalid state")?.1),
        }
            cnt += 1;
            i = (i+1) % self.dirs.len();
        }
        Ok(cnt)
    }

}

impl Day for Day08 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day08.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day08.dat.testing").context("reading input file failed")?;
        // let file = File::open("./problems/day08.dat.testing2").context("reading input file failed")?;
        let buf = io::BufReader::new(file);

        let mut lines = buf.lines();
        let dirs = lines.next().context("faled retreiving the first line")??;
        self.dirs = dirs.chars().map(|x| <char as TryInto<Dir>>::try_into(x)).collect::<Result<_>>()?;
        let _ = lines.next();

        for l in lines {
            let l = l?;
            let (k,vs) = l.split_once(" = ").context("failed to parse graph node")?;
            let (l,r) = vs
                .strip_prefix("(")
                .context("failed to parse neighbours (1)")?
                .strip_suffix(")")
                .context("failed to parse neighbours (2)")?
                .split_once(", ").context("failed to parse neighbours (3)")?;

            self.graph.insert(k.to_string(), (l.to_string(), r.to_string(), k.ends_with("Z")));
        }

        // println!("{self:#?}");

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        Ok(self.follow("AAA", |x| x == "ZZZ")?.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        // that's a nice implementation but exploits many invariants like when hitting the first
        // destination node:
        // - the next node will be again the starting node => no loops with more than one destination node possible
        // - the index in the instruction array will be at array.len
        //
        // Part of this might be tackled by iterating over combinations of destination nodes in
        // different cycles and using the CRT (see https://www.reddit.com/r/adventofcode/comments/18df7px/comment/kch1mrr/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)
        // and breaking up the nice structure below by just applying follow and don't check what the next node and the instruction index is

        Ok(self.graph
            .keys()
            .filter(|k| k.ends_with("A"))
            .map(|x| self.follow(x, |x:&str| x.ends_with("Z")))
            .reduce(|a,b| {
                a.and_then(|a| b.map(|b| lcm(a,b)))
            }).context("reduce failed")??.to_string()
        )
    }
}

impl Default for Day08 {
    fn default() -> Self {
        Self::new()
    }
}

fn gcd(a: usize, b: usize) -> usize {
    let mut a = a;
    let mut b = b;

    while b != 0 {
        let temp = b;
        b = a % b;
        a = temp;
    }

    a
}

fn lcm(a: usize, b: usize) -> usize {
    if a == 0 || b == 0 {
        0
    } else {
        (a * b) / gcd(a, b)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
    }
}
