use day_trait::Day;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result};

pub struct Day01 {
    lines: Vec<String>,
}

impl Day01 {
    pub fn new() -> Day01 {
        Day01{
            lines: vec![],
        }
    }
}

impl Day for Day01 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day01.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day01.dat.testing").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        for line in buf.lines() {
            self.lines.push(line?);
        }
        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;
        for l in &self.lines {
            let mut first:i32 = 0;
            let mut last  = 0;
            for c in l.chars() {
                if c.is_digit(10) {
                    if first == -1 {
                        first = c.to_digit(10).context("char to int failed")? as i32;
                    }
                    last = c.to_digit(10).context("char to int failed")?;
                }
            }
            ret += (first as u32)*10 + last;
        }

        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        let mut ret = 0;
        for l in &mut self.lines {

            replace(l)?;

            let mut first:i32 = -1;
            let mut last  = 0;
            for c in l.chars() {
                if c.is_digit(10) {
                    if first == -1 {
                        first = c.to_digit(10).context("char to int failed")? as i32;
                    }
                    last = c.to_digit(10).context("char to int failed")?;
                }
            }
            ret += (first as u32)*10 + last;
        }

        Ok(ret.to_string())
    }
}

fn replace(input: &mut String) -> Result<()> {
    const DIGITS:[&str;10] = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];
    let mut is:[(Option<usize>,Option<usize>);DIGITS.len()] = [(None,None);DIGITS.len()];

    for (i,d) in DIGITS.iter().enumerate() {
        if let Some(j) = input.find(d) {
            is[i].0 = Some(j);
        }
        if let Some(j) = input.rfind(d) {
            is[i].1 = Some(j+d.len());
        }
    }

    let first = is
        .iter()
        .enumerate()
        .filter(|(_,(x,_))| x.is_some())
        .map(|(i,(x,_))| (i,x.unwrap()))
        .min_by(|(_,a),(_,b)| a.cmp(&b))
        .map(|(d_i,idx)| (d_i,idx));

    let last = is
        .iter()
        .enumerate()
        .filter(|(_,(_,x))| x.is_some())
        .map(|(i,(_,x))| (i,x.unwrap()))
        .max_by(|(_,a),(_,b)| a.cmp(&b))
        .map(|(d_i,idx)| (d_i,idx));

    if let Some(first) = first {
        let first_range = first.1..(first.1+DIGITS[first.0].len());
        if let Some(last) = last {
            let last_range = (last.1-DIGITS[last.0].len())..last.1;

            if first_range.start < last_range.end && last_range.start < first_range.end {
                // overlapping
                input.replace_range(first_range.start..last_range.end, &(first.0.to_string() + &last.0.to_string()));
            } else {
                // non overlapping
                input.replace_range(last_range.clone(), &last.0.to_string());
                if first_range != last_range {
                    input.replace_range(first_range, &first.0.to_string());
                }
            }
        } else {
            input.replace_range(first_range, &first.0.to_string());
        }
    }



    Ok(())
}

impl Default for Day01 {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works_b() {
        let mut x = "oneeight".to_string();
        replace(&mut x).unwrap();
        assert_eq!(x, "18");
    }

    #[test]
    fn it_works_a() {
        let mut x = "oneight".to_string();
        replace(&mut x).unwrap();
        assert_eq!(x, "18");
    }
}
