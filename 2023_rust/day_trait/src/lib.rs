use anyhow::Result;

pub trait Day {
    fn parse(&mut self) -> Result<()>;
    fn solve_part1(&mut self) -> Result<String>;
    fn solve_part2(&mut self) -> Result<String>;
}
