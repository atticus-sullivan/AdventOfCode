use day_trait::Day;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result};
// use regex::Regex;

const DIGITS:[&str;10] = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];

#[derive(Clone,Debug)]
pub struct Day01 {
    lines: Vec<String>,
}

impl Day01 {
    pub fn new() -> Day01 {
        Day01{
            lines: vec![],
        }
    }

    pub fn solve_part2_replace(&mut self) -> Result<String> {
        let mut ret = 0;
        for l in &mut self.lines {

            // let l_old = l.clone();
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
            // println!("{l_old} {}", (first as u32)*10 + last);
            ret += (first as u32)*10 + last;
        }

        Ok(ret.to_string())
    }

    pub fn solve_part2_collect(&mut self) -> Result<String> {
        let mut ret = 0;

        for l in &self.lines {
            let (first,last) = collect(l);
            // println!("{l} -> {}", first*10 + last);
            ret += first*10 + last;
        }

        Ok(ret.to_string())
    }

    // pub fn solve_part2_regex(&mut self) -> Result<String> {
    //     let mut ret = 0;
    //
    //     for l in &self.lines {
    //         let (first,last) = regex(l);
    //         // println!("{l} -> {}", first*10 + last);
    //         ret += first*10 + last;
    //     }
    //
    //     Ok(ret.to_string())
    // }

    // needed for benchmarking
    pub fn parse_with_prefix(&mut self, prefix: &str) -> Result<()> {
        let file = File::open(prefix.to_string() + "problems/day01.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day01.dat.testing").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        for line in buf.lines() {
            self.lines.push(line?);
        }
        Ok(())
    }
}

impl Day for Day01 {
    fn parse(&mut self) -> Result<()> {
        Ok(self.parse_with_prefix("./")?)
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
        // Ok(self.solve_part2_replace()?)
        Ok(self.solve_part2_collect()?)
    }
}

// FIXME: This doesn't work because no rfind for regex exists
// fn regex(input: &String) -> (u32,u32) {
//     let re_first = Regex::new(r"one|two|three|four|five|six|seven|eight|nine|1|2|3|4|5|6|7|8|9").unwrap();
//     let first = re_first.find(input).unwrap();
//
//     // FIXME: This doesn't work
//     let re_last = Regex::new(r"^.*(one|two|three|four|five|six|seven|eight|nine|1|2|3|4|5|6|7|8|9)").unwrap();
//     let last = re_last.find(input).unwrap();
//
//     let mut first_num = 0;
//     match first.as_str() {
//         "one"|"1" => first_num = 1,
//         "two"|"2" => first_num = 2,
//         "three"|"3" => first_num = 3,
//         "four"|"4" => first_num = 4,
//         "five"|"5" => first_num = 5,
//         "six"|"6" => first_num = 6,
//         "seven"|"7" => first_num = 7,
//         "eight"|"8" => first_num = 8,
//         "nine"|"9" => first_num = 9,
//         _ => panic!("invalid match"),
//     }
//     let mut last_num = 0;
//     match last.as_str() {
//         "one"|"1" => last_num = 1,
//         "two"|"2" => last_num = 2,
//         "three"|"3" => last_num = 3,
//         "four"|"4" => last_num = 4,
//         "five"|"5" => last_num = 5,
//         "six"|"6" => last_num = 6,
//         "seven"|"7" => last_num = 7,
//         "eight"|"8" => last_num = 8,
//         "nine"|"9" => last_num = 9,
//         _ => panic!("invalid match"),
//     }
//
//     (first_num,last_num)
// }

fn collect(input: &String) -> (u32,u32) {
    let mut first:(usize,u32) = (usize::MAX,0);
    let mut last:(usize,u32) = (0,0);

    for (i,d) in DIGITS.iter().enumerate() {
        if let Some(j) = input.find(d) {
            if j < first.0 {
                first.0 = j;
                first.1 = i as u32;
            }
        }
        if let Some(j) = input.rfind(d) {
            let j = j+d.len()-1;
            if j > last.0 {
                last.0 = j;
                last.1 = i as u32;
            }
        }
    }
    for (i,c) in input.chars().enumerate() {
        if let Some(c) = c.to_digit(10) {
            if i < first.0 {
                first.0 = i;
                first.1 = c;
            }
            // need to check for equal because 0 is initial
            if i >= last.0 {
                last.0 = i;
                last.1 = c;
            }
        }
    }
    (first.1,last.1)
}

fn replace(input: &mut String) -> Result<()> {
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
    fn replace_non_overlap() {
        let mut x = "oneeight".to_string();
        replace(&mut x).unwrap();
        assert_eq!(x, "18");
    }

    #[test]
    fn replace_overlap() {
        let mut x = "oneight".to_string();
        replace(&mut x).unwrap();
        assert_eq!(x, "18");
    }

    #[test]
    fn collect_non_overlap() {
        let x = "oneeight".to_string();
        let (first,last) = collect(&x);
        assert_eq!(first, 1);
        assert_eq!(last, 8);
    }

    #[test]
    fn collect_overlap() {
        let x = "oneight".to_string();
        let (first,last) = collect(&x);
        assert_eq!(first, 1);
        assert_eq!(last, 8);
    }

    #[test]
    fn collect_single_digit() {
        let x = "4fefw".to_string();
        let (first,last) = collect(&x);
        assert_eq!(first, 4);
        assert_eq!(last, 4);
    }
}
