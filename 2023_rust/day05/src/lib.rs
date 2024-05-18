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
use std::cmp::Ordering;
use std::collections::{BTreeMap,HashMap};
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result, anyhow};
use regex::Regex;

#[derive(Debug,Clone)]
struct Map {
    from: String,
    to: String,
    // idea was once to use a lower/upper bound query but rust doesn't support this as of now ->
    // chose to simply iterate over the elements. Might be using a Vector but still it is some sort
    // of mapping
    ranges: BTreeMap<Range, u64>,
}

#[derive(Debug,Clone)]
struct Range {
    start: u64,
    end: u64,
}
impl Range {
    fn contains(&self, item: u64) -> bool {
        self.start <= item && self.end >= item
    }
    fn overlaps(&self, other: &Self) -> bool {
        self.start <= other.end && other.start <= self.end
    }
}
impl Ord for Range {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.start.cmp(&other.start)
    }
}
impl PartialEq for Range {
    fn eq(&self, other: &Self) -> bool {
        self.start.eq(&other.start)
    }
}
impl Eq for Range{}
impl PartialOrd for Range {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

#[derive(Debug,Clone)]
pub struct Day05 {
    seeds: Vec<u32>,
    maps: HashMap<String,Map>,
}

impl Day05 {
    pub fn new() -> Day05 {
        Day05{
            seeds: vec![],
            maps: HashMap::new(),
        }
    }
}

impl Day for Day05 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day05.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day05.dat.testing").context("reading input file failed")?;
        let buf = io::BufReader::new(file);
        // pattern to match the from-to-dst string which is the header of each list of mappings
        let pattern = Regex::new(r"^(.*?)-to-(.*?) map:$")?;
        let mut lines = buf.lines();

        let seeds = lines.next().context("no seeds found")??;
        self.seeds = seeds
            .strip_prefix("seeds: ")
            .context("stripping \"seeds\" failed")?
            .trim()
            .split(" ")
            .map(|x| x.parse().context("parsing seed failed"))
            .collect::<Result<_>>()?;

        let mut m:Option<Map> = None; // store the map which currently is being collected
        while let Some(l) = lines.next() {
            let l = l?;
            if l == "" {
                match m {
                    Some(m) => {
                        // insert previous gathered map
                        self.maps.insert(m.from.clone(), m);
                    },
                    None => {},
                }
                // parse header of next map
                let l = lines.next().context("no line after empty one")??;
                let c = pattern.captures(&l).context("pattern matching failed with line {l}")?;
                m = Some(Map{
                    from: c[1].to_string(),
                    to: c[2].to_string(),
                    ranges: BTreeMap::new(),
                });
                continue // no mapping to be read in this iteration
            }

            // read the mapping ranges
            let x:Vec<u64> = l.trim().split(" ").map(|x| x.parse().context("invalid num in ranges")).collect::<Result<_>>()?;
            match x.len() {
                3 => {
                    match &mut m {
                        Some(m) => {
                            let r = Range{start:x[1],end:x[1]+x[2]-1};
                            // merge overlapping ranges?
                            // let lu = get_lower_upper(&mut m.ranges, &r);
                            // let mut insert = true;
                            // match lu.0 {
                            //     Some(l) => {
                            //         if l.overlaps(&r) {
                            //             // TODO merge them
                            //         }
                            //     }
                            //     None => {},
                            // }
                            m.ranges.insert(r, x[0]);
                        },
                        None => return Err(anyhow!("no map but ranges to read")),
                    }
                },
                _ => return Err(anyhow!("Invalid ranges line read {l} -> {x:?}")),
            }
        }

        // insert final map too
        match m {
            Some(m) => {
                self.maps.insert(m.from.clone(), m);
            },
            None => {},
        }

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = u64::MAX;

        for s in &self.seeds {
            let mut state = "seed".to_string(); // state stored as strings to be flexible with the input data
            // seed, soil, ... which is to be mapped
            let mut num = *s as u64;

            // map num until the location was calculated
            while state != "location" {
                // get the map which maps the current state to the next one
                let map = self.maps.get_mut(&state).context("map {state} not found")?;

                // search for a suitable mapping
                for (k,v) in &map.ranges {
                    if k.contains(num) {
                        num = num - k.start + v; // do the actual mapping
                        break;
                    }
                }

                // make things ready for mapping to the next state
                state = map.to.to_string();
            }
            // search for the smallest location (which num is in the end of the series of mappings)
            ret = ret.min(num);
        }

        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        let mut ret = u64::MAX;

        // use indices to support zip with stride=2
        for s in 0..self.seeds.len()/2 {
            let mut state = "seed".to_string(); // state stored as strings to be flexible with the input data
            // construct ranges which need to be processed. Initially this is only the seeds range
            let mut ranges = vec![Range{start:self.seeds[s*2] as u64, end: (self.seeds[s*2]+self.seeds[s*2+1]-1) as u64}];

            // map ranges until the location was calculated
            while state != "location" {
                // get the map which maps the current state to the next one
                let map = self.maps.get_mut(&state).context("map {state} not found")?;
                // collect the output ranges of the current state-mapping
                let mut ranges_new = vec![];

                // try to map each range which currently needs to be processed
                for r in &mut ranges {
                    let mut leftover = true; // figure out if the range was already fully mapped

                    // search for a suitable mapping
                    for (k,v) in &map.ranges {
                        // if there is no overlap there is no point in checking edge cases
                        if k.overlaps(r) {
                            if r.start < k.start {
                                //        [k.start k.end]
                                // [r.start r.end]
                                // range is only partly contained -> split it up into mapped range (ranges_new) and the remaining part
                                ranges_new.push(Range{start:*v, end:v+(r.end-k.start)}); // map [k.start, r.end]
                                r.end = k.start-1;
                            } else if r.end > k.end {
                                // [k.start k.end]
                                //       [r.start r.end]
                                // range is only partly contained -> split it up into mapped range (ranges_new) and the remaining part
                                ranges_new.push(Range{start:v+(r.start-k.start), end:v+(k.end-k.start)}); // map [r.start, k.end]
                                r.start = k.end+1;
                            } else {
                                // r is completely contained in k
                                ranges_new.push(Range{start:v+(r.start-k.start), end:v+(r.end-k.start)}); // map [r.start, r.end]
                                leftover = false;
                                break // no point in continuing to search if r is already fully mapped
                            }
                        }
                    }

                    if leftover {
                        // identity translation
                        ranges_new.push(Range{start:r.start, end:r.end});
                    }
                }

                // make things ready for mapping to the next state
                ranges = ranges_new;
                state = map.to.to_string();
            }

            // search for the range with the smallest start value (smallest location)
            ret = ret.min(ranges.iter().map(|x| x.start).min().context("no min in ranges found")?);
        }

        Ok(ret.to_string())
    }
}

impl Default for Day05 {
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
