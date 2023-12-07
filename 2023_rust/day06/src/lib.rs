use day_trait::Day;
use std::fs;
use anyhow::{Context, Result};

#[derive(Debug,Clone)]
pub struct Day06 {
    times: Vec<u32>,
    distances: Vec<u32>,

    time: u64,
    distance: u64,
}

impl Day06 {
    pub fn new() -> Day06 {
        Day06{
            // data for part 1
            times: vec![],
            distances: vec![],
            // data for part 2
            time: 0,
            distance: 0,
        }
    }
}

impl Day for Day06 {
    fn parse(&mut self) -> Result<()> {
        let filename = "./problems/day06.dat";
        // let filename = "./problems/day06.dat.testing";

        // read data for part 1
        let file = fs::read_to_string(filename).context("reading input file failed")?;
        let (time, distance) = file.split_once("\n").context("reading time and distance failed")?;
        self.times = time
            .strip_prefix("Time:")
            .context("failed to parse times")?
            .trim()
            .split_whitespace()
            .map(|x| x.parse().context("failed parsing time ele {x:?}"))
            .collect::<Result<_>>()?;
        self.distances = distance
            .strip_prefix("Distance:")
            .context("failed to parse distances")?
            .trim()
            .split_whitespace()
            .map(|x| x.parse().context("failed parsing distance ele {x:?}"))
            .collect::<Result<_>>()?;

        // read data for part 2
        let file = fs::read_to_string(filename).context("reading input file failed")?;
        let (time, distance) = file.split_once("\n").context("reading time and distance failed")?;
        self.time = time
            .strip_prefix("Time:")
            .context("failed to parse time")?
            .replace(" ", "")
            .parse().context("failed parsing time")?;
        self.distance = distance
            .strip_prefix("Distance:")
            .context("failed to parse distance")?
            .trim()
            .replace(" ", "")
            .parse().context("failed parsing distance")?;

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 1;
        for (t,d) in std::iter::zip(&self.times, &self.distances) {
            ret *= calc_possibilities3(*d, *t);
        }
        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        Ok(calc_possibilities6(self.distance, self.time).to_string())
    }
}

/// The formula to calculate the amount of time invested in the acceleration is
/// d = a*t - a^2 => a_{1,2} = (N +- sqrt(N^2 - 4d)) / 2
/// As the d = a*(t-a) is quadratic, there will be two durations used for acceleration which
/// result in the highscore distance (which we calculate with the first formula).
/// We calculate this and the amount of different possible
/// accelerations in between these two points are the accelerations which can be used to beat
/// the highscore.
fn calc_possibilities3(d: u32, t: u32) -> u32 {
    // next smaller whole number so use -1 and then ceil
    let a_score_1 = ((t as f64 + f64::sqrt((t*t - 4*d) as f64)) / 2.0 - 1.0).ceil() as u32;
    // next larger whole number so use +1 and the floor
    let a_score_2 = ((t as f64 - f64::sqrt((t*t - 4*d) as f64)) / 2.0 + 1.0).floor() as u32;

    a_score_1 - a_score_2 + 1
}
/// The formula to calculate the amount of time invested in the acceleration is
/// d = a*t - a^2 => a_{1,2} = (N +- sqrt(N^2 - 4d)) / 2
/// As the d = a*(t-a) is quadratic, there will be two durations used for acceleration which
/// result in the highscore distance (which we calculate with the first formula).
/// We calculate this and the amount of different possible
/// accelerations in between these two points are the accelerations which can be used to beat
/// the highscore.
fn calc_possibilities6(d: u64, t: u64) -> u64 {
    // next smaller whole number so use -1 and then ceil
    let a_score_1 = ((t as f64 + f64::sqrt((t*t - 4*d) as f64)) / 2.0 - 1.0).ceil() as u64; // next smaller whole number
    // next larger whole number so use +1 and the floor
    let a_score_2 = ((t as f64 - f64::sqrt((t*t - 4*d) as f64)) / 2.0 + 1.0).floor() as u64; // next larger whole number

    a_score_1 - a_score_2 + 1
}

impl Default for Day06 {
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
