use day_trait::Day;
use std::collections::{VecDeque, HashMap, HashSet, BTreeSet};
use std::fmt::Display;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result, anyhow};
use std::convert::TryFrom;
use std::hash::Hash;

#[derive(Clone,Debug,Copy,PartialEq)]
enum FieldEle {
    NS,
    EW,
    NE,
    NW,
    SW,
    SE,
    Gnd,
    Start,
}

#[derive(Clone,Debug,Copy,PartialEq,Hash,Eq)]
struct Dir(isize,isize);

impl FieldEle {
    fn connected_adjecent(&self) -> Vec<Dir> {
        match self {
            FieldEle::NS => vec![Dir(0, 1), Dir( 0,-1)],
            FieldEle::EW => vec![Dir(1, 0), Dir(-1, 0)],
            FieldEle::NE => vec![Dir(0,-1), Dir( 1, 0)],
            FieldEle::NW => vec![Dir(0,-1), Dir(-1, 0)],
            FieldEle::SW => vec![Dir(0, 1), Dir(-1, 0)],
            FieldEle::SE => vec![Dir(0, 1), Dir( 1, 0)],
            FieldEle::Gnd => vec![],
            FieldEle::Start => vec![Dir(0,1), Dir(0,-1), Dir(1,0), Dir(-1,0)],
        }
    }
    fn unconnected_adjacent(&self) -> Vec<Dir> {
        match self {
            FieldEle::NS => vec![Dir(1, 0), Dir(-1, 0)],
            FieldEle::EW => vec![Dir(0, 1), Dir( 0, -1)],
            FieldEle::NE => vec![Dir(0, 1), Dir(-1, 0)],
            FieldEle::NW => vec![Dir(0, 1), Dir( 1, 0)],
            FieldEle::SW => vec![Dir(0,-1), Dir( 1, 0)],
            FieldEle::SE => vec![Dir(0,-1), Dir(-1, 0)],
            FieldEle::Gnd => vec![Dir(0,1), Dir(0,-1), Dir(1,0), Dir(-1,0)],
            FieldEle::Start => vec![],
        }
    }
    fn connects_to(&self, other: &Self, dir: &Dir) -> bool {
        let o = other.connected_adjecent();
        let s = self.connected_adjecent();

        s.contains(dir) && o.contains(&Dir(-dir.0, -dir.1))
    }
}

// conversion from char
impl TryFrom<char> for FieldEle {
    type Error = anyhow::Error;
    fn try_from(value:char) -> std::result::Result<Self, Self::Error> {
        match value {
            '|' => Ok(FieldEle::NS),
            '-' => Ok(FieldEle::EW),
            'L' => Ok(FieldEle::NE),
            'J' => Ok(FieldEle::NW),
            '7' => Ok(FieldEle::SW),
            'F' => Ok(FieldEle::SE),
            '.' => Ok(FieldEle::Gnd),
            'S' => Ok(FieldEle::Start),
            _ => Err(anyhow!("invalid char {value}")),
        }
    }
}

// conversion based on outgoing connections (no for 'S' or '.')
impl TryFrom<[Dir;2]> for FieldEle {
    type Error = anyhow::Error;
    fn try_from(value:[Dir;2]) -> Result<Self, Self::Error> {
        match value {
            [Dir(0, 1), Dir( 0,-1)] => Ok(FieldEle::NS),
            [Dir(1, 0), Dir(-1, 0)] => Ok(FieldEle::EW),
            [Dir(0,-1), Dir( 1, 0)] => Ok(FieldEle::NE),
            [Dir(0,-1), Dir(-1, 0)] => Ok(FieldEle::NW),
            [Dir(0, 1), Dir(-1, 0)] => Ok(FieldEle::SW),
            [Dir(0, 1), Dir( 1, 0)] => Ok(FieldEle::SE),
            _ => Err(anyhow!("invalid pos {value:?}")),
        }
    }
}

#[derive(Clone,Debug)]
pub struct Day10 {
    field: (Vec<FieldEle>,Option<usize>),
    start: Option<usize>,
    cycle_result: Option<(HashMap<usize,usize>,(usize,usize))>,
}

impl Day10 {
    pub fn new() -> Day10 {
        Day10{
            field: (vec![],None),
            start: None,
            cycle_result: None,
        }
    }
    fn get_cycle_result(&mut self) -> Option<(HashMap<usize,usize>,(usize,usize))> {
        match self.cycle_result {
            Some(_) => self.cycle_result.clone(),
            None => {
                match self.solve_part1() {
                    Err(_) => None,
                    Ok(_) => self.cycle_result.clone(),
                }
            },
        }
    }
}

impl Day for Day10 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day10.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day10.dat.testing").context("reading input file failed")?;
        // let file = File::open("./problems/day10.dat.testing2B").context("reading input file failed")?;

        let buf = io::BufReader::new(file);
        for line in buf.lines() {
            let line = line?;
            for c in line.chars() {
                self.field.0.push(<char as TryInto<FieldEle>>::try_into(c)?);

                if (<char as TryInto<FieldEle>>::try_into(c)?) == FieldEle::Start {
                    match self.start {
                        Some(_) => return Err(anyhow!("multiple starts found")),
                        None => self.start = Some(self.field.0.len()-1),
                    }
                }
            }
            match self.field.1 {
                Some(x) => {
                    if x != line.len() {
                        return Err(anyhow!("Not all lines with same length"))
                    }
                },
                None => self.field.1 = Some(line.len()),
            }
        }

        // println!("{self:?}");

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        // enforce that both start and stride are being set
        match (self.start,self.field.1) {
            (Some(start),Some(stride)) => {

                // cary out bfs-like approach to find the cycle
                let (parents,connect) = find_cycle(&start, &start, |v:&usize| -> Vec<usize> {
                    // get a vector of neighbouring indices
                    match self.field.0.get(*v) {
                        Some(f) => f.connected_adjecent()
                            .iter()
                            .filter_map(|Dir(x,y)| {
                                // check if coordinates are valid and map them to (x,y) coordinate tuple
                                match ((v%stride).checked_add_signed(*x), (v/stride).checked_add_signed(*y)) {
                                    (Some(nx),Some(ny)) if nx < stride && ny < self.field.0.len()/stride => {
                                        // calculate new index
                                        let w = nx + ny * stride;
                                        // check if new field (w/u) is connected with curent field (v)
                                        self.field.0.get(w).and_then(|u| {
                                            if u.connected_adjecent().contains(&Dir(-x,-y)) {
                                                Some(w)
                                            } else {
                                                None
                                            }
                                        })
                                    },
                                    _ => None
                                }
                            }).collect(),
                        None => vec![],
                    }
                })?;

                // start at one end of the cycle and check how many hops it takes to get to the start node
                let mut n = parents.get(&connect.0).context("connection point A of cycle must be in parents")?;
                let mut ret = 1;
                while n != &start {
                    n = parents.get(n).context("parents is not connected")?;
                    ret += 1;
                }
                // store found cycle for later use
                self.cycle_result = Some((parents,connect));
                Ok(ret.to_string())
            },
            _ => Err(anyhow!("start and stride must be set"))
        }
    }

    fn solve_part2(&mut self) -> Result<String> {
        // make sure a cycle was found and start+stride are being set
        match (self.get_cycle_result(), self.start, self.field.1) {
            (Some((parents,connect)),Some(start),Some(stride)) => {

                // The idea is to follow the fields contained in the cycle. For each field check at the
                // sides which aren't connected wether the field is enclosed by the loop. If so,
                // follow a greedy approach (basically bfs without searching) to find all other
                // fields contained in this connected (enclosed by cycle fields) area.
                // Set all fields which already were checked as visited to reduce runtime and avoid
                // double counting fields.
                // Especially to find out wether a field is enclosed by the cycle we build up two
                // cycle look-up map. One to iterate (ordered) over the cycle fields all with the
                // same y-coordinate and one for the x-coordinate.
                // To handle the start field correctly, we check in which direction it is connected
                // and kinda replace it with the corresponding FieldEle.

                let mut visited = HashSet::new();
                // collect set of indics contained in the cycle and figure out which connections
                // are needed for the start field.
                let (cycle,s_n) = build_cycle(&parents, &connect, &start)?;
                let start_repl = <[Dir;2] as TryInto<FieldEle>>::try_into(s_n.map(|n| {
                    Dir((n%stride) as isize - (start%stride) as isize, (n/stride) as isize - (start/stride) as isize)
                }))?;

                // build up these x-/y-Coordinate based LUTs (one with x->y and one with y->x,
                // these are ordered in the second dimension)
                let cycle_lut = build_cycle_lut(&cycle, stride);

                let mut ret = 0;
                for c in &cycle {
                    // get the currently worked on field (with the start field replacement)
                    let f = if c == &start {
                        &start_repl
                    } else {
                        self.field.0.get(*c).context("field not found")?
                    };

                    // get fields which are adjacent but unconected as x,y coordinates
                    for u in f.unconnected_adjacent()
                        .iter()
                        .filter_map(|Dir(x,y)| {
                            // check if coordinates are valid and map them to (x,y) coordinate tuple
                            match ((c%stride).checked_add_signed(*x), (c/stride).checked_add_signed(*y)) {
                                (Some(nx),Some(ny)) => {
                                    if nx < stride && ny < self.field.0.len()/stride {
                                        Some((nx,ny))
                                    } else {
                                        None
                                    }
                                },
                                _ => None
                            }
                        }) {
                            // skip already visited ones and skip cycle fields (obviously not enclosed)
                            if visited.contains(&u) || cycle.contains(&(u.0 as usize + u.1 as usize*stride)) {
                                continue;
                            }
                            // if the field is not enclosed skip it and mark as visite to avoid
                            // checking for enclosed again
                            if !enclosed((&self.field.0,stride), &cycle_lut, &u).unwrap_or(false) {
                                visited.insert(u);
                                continue;
                            }
                            // greedy count + set visited (bfs-like)
                            let mut q = VecDeque::new();
                            q.push_back(u);
                            // since areas are closed and connected, using the global visited is no problem
                            visited.insert(u);
                            while !q.is_empty() {
                                let v = q.pop_front().context("queue is empty")?;
                                ret += 1;
                                // go in all directions
                                // don't check bounds since this region should be enclosed by cycle
                                // and never get out of bounds
                                for w in [(v.0+1,v.1), (v.0-1,v.1), (v.0,v.1+1), (v.0,v.1-1)] {
                                    match visited.get(&w) {
                                        Some(_) => {},
                                        None => {
                                            if w.0 < stride && w.1 < (self.field.0.len()/stride) && !cycle.contains(&(w.0 as usize + w.1 as usize*stride)) {
                                                visited.insert(w);
                                                q.push_back(w);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                }

                Ok(ret.to_string())
            },
            _ => Err(anyhow!("fetching bfs_result failed")),
        }
    }
}

fn build_cycle_lut(cycles: &HashSet<usize>, stride: usize) -> (HashMap<usize, BTreeSet<usize>>,HashMap<usize, BTreeSet<usize>>) {
    let mut ret = (HashMap::<usize,BTreeSet<_>>::new(), HashMap::<usize,BTreeSet<_>>::new());
    for c in cycles {
        let (x,y) = (c%stride, c/stride);
        ret.0.entry(x).or_default().insert(y);
        ret.1.entry(y).or_default().insert(x);
    }
    ret
}

fn enclosed(field: (&Vec<FieldEle>,usize), cycle_eles: &(HashMap<usize, BTreeSet<usize>>,HashMap<usize, BTreeSet<usize>>), pos: &(usize,usize)) -> Result<bool> {
    let (x,y) = pos;
    // check vertical
    let mut down = 0;
    let mut up = 0;
    {
        let mut last = (FieldEle::Gnd, 0, (false,false));
        for cy in cycle_eles.0.get(x).context("x not found")?.iter() {
            let f = field.0.get(cy*field.1 + x).context("ele not found")?;

            if (cy - last.1) > 1 || !f.connects_to(&last.0, &Dir(0,-1)) {
                // not connected -> count last occurence
                if &last.1 > y && (last.2.0 && last.2.1){
                    down += 1;
                } else if &last.1 < y && (last.2.0 && last.2.1) {
                    up += 1;
                }
                last.2 = (false,false);
            }

            match f {
                FieldEle::NW | FieldEle::SW => last.2.0 = true,
                FieldEle::NE | FieldEle::SE => last.2.1 = true,
                FieldEle::EW => last.2 = (true,true),
                _ => {},
            }

            last = (*f, *cy, last.2);
        }
        if &last.1 > y && (last.2.0 && last.2.1){
            down += 1;
        } else if &last.1 < y && (last.2.0 && last.2.1) {
            up += 1;
        }
    }

    // check horizontal
    let mut right = 0;
    let mut left = 0;
    {
        let mut last = (FieldEle::Gnd, 0, (false,false));
        for cx in cycle_eles.1.get(y).context("y not found")?.iter() {
            let f = field.0.get(y*field.1 + cx).context("ele not found")?;

            if (cx - last.1) > 1 || !f.connects_to(&last.0, &Dir(-1,0)) {
                // not connected -> count last occurence
                if &last.1 > x && (last.2.0 && last.2.1) {
                    right += 1;
                } else if &last.1 < x && (last.2.0 && last.2.1) {
                    left += 1;
                }
                last.2 = (false,false);
            }

            match f {
                FieldEle::NW | FieldEle::NE => last.2.0 = true,
                FieldEle::SW | FieldEle::SE => last.2.1 = true,
                FieldEle::NS => last.2 = (true,true),
                _ => {},
            }

            last = (*f, *cx, last.2);
        }
        if &last.1 > x && (last.2.0 && last.2.1) {
            right += 1;
        } else if &last.1 < x && (last.2.0 && last.2.1) {
            left += 1;
        }
    }

    Ok(down.min(up).min(left).min(right) % 2 == 1)
}

fn build_cycle(parents: &HashMap<usize,usize>, connect: &(usize,usize), start: &usize) -> Result<(HashSet<usize>,[usize;2])> {
    let mut ret = HashSet::new();
    let mut start_neighbours = [None;2];
    // first+second branch
    // second branch
    for (i,con) in [connect.0,connect.1].iter().enumerate() {
        let mut cur = con;
        ret.insert(*cur);
        while cur != start {
            start_neighbours[i] = Some(*cur);
            cur = parents.get(cur).context("parents is not connected")?;
            ret.insert(*cur);
        }
    }
    match start_neighbours.into_iter().collect::<Option<Vec<_>>>().and_then(|a| a.try_into().ok()) {
        Some(s_n) => Ok((ret,s_n)),
        None => Err(anyhow!("not enough neighbour for start found")),
    }
}

fn find_cycle<T,GN>(root: &T, dst: &T, get_neighbours: GN) -> Result<(HashMap<T,T>,(T,T))>
where
    T: Clone+Ord+Hash+Display,
    GN: Fn(&T) -> Vec<T>,
{
    let mut q = VecDeque::new();
    let mut explored = HashSet::new();
    let mut parents = HashMap::new();

    explored.insert(root.clone());
    q.push_back(root.clone());
    let mut i = 0;
    while !q.is_empty() {
        let v = q.pop_front().context("queue shouldn't be empty here")?;
        if i > 0 && &v == dst {
            return Err(anyhow!("No loop connected!"))
        }
        i += 1;
        let n = get_neighbours(&v);
        for w in n.iter() {
            match parents.get(&v) {
                Some(x) => {
                    if x == w {
                        continue;
                    }
                },
                None => {},
            }
            match explored.get(w) {
                None => {
                    explored.insert(w.clone());
                    parents.insert(w.clone(), v.clone());
                    q.push_back(w.clone());
                },
                Some(_) => {
                    if w != root {
                        return Ok((parents,(w.clone(),v.clone())));
                    }
                },
            }
        }
    }
    Err(anyhow!("Dst was not found"))
}

impl Default for Day10 {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    // depends on test input -> don't do this for usual testing because the PWD is not right
    fn it_works() -> Result<()> {
        // let mut d = Day10::new();
        // d.parse()?;
        // d.solve_part1()?;
        // match (d.get_cycle_result(),d.start,d.field.1) {
        //     (Some((parents,connect)),Some(start),Some(stride)) => {
        //         let (cycle,_) = build_cycle(&parents, &connect, &start)?;
        //         let cycle_lut = build_cycle_lut(&cycle, stride);
        //         assert_eq!(enclosed((&d.field.0, stride), &cycle_lut, &(4,4))?, false);
        //         assert_eq!(enclosed((&d.field.0, stride), &cycle_lut, &(3,2))?, false);
        //         assert_eq!(enclosed((&d.field.0, stride), &cycle_lut, &(6,6))?, true);
        //     },
        //     _ => return Err(anyhow!("invalid state")),
        // }
        Ok(())
    }
}
