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
use std::collections::BTreeMap;
use std::fs::File;
use std::io::{self, BufRead};
use anyhow::{Context, Result, anyhow};
use std::convert::TryFrom;
use std::cmp::{Ord,PartialOrd,PartialEq,Eq,Ordering};
use std::hash::Hash;

#[derive(Debug,Clone,Copy,PartialOrd,Ord,PartialEq,Eq,Hash)]
enum Card {
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    T = 10,
    J = 11,
    Q = 12,
    K = 13,
    A = 14,
}

impl TryFrom<char> for Card {
    type Error = anyhow::Error;
    fn try_from(v: char) -> Result<Self,Self::Error> {
        match v {
            x if x == '2' => Ok(Card::TWO),
            x if x == '3' => Ok(Card::THREE),
            x if x == '4' => Ok(Card::FOUR),
            x if x == '5' => Ok(Card::FIVE),
            x if x == '6' => Ok(Card::SIX),
            x if x == '7' => Ok(Card::SEVEN),
            x if x == '8' => Ok(Card::EIGHT),
            x if x == '9' => Ok(Card::NINE),
            x if x == 'T' => Ok(Card::T),
            x if x == 'J' => Ok(Card::J),
            x if x == 'Q' => Ok(Card::Q),
            x if x == 'K' => Ok(Card::K),
            x if x == 'A' => Ok(Card::A),
            _ => Err(anyhow!("Invalid char {v}")),
        }
    }
}

#[derive(Debug,Clone,Copy,Ord,PartialOrd,Eq,PartialEq)]
enum HandType {
    HighCard,
    OnePair,
    TwoPair,
    ThreeKind,
    FullHouse,
    FourKind,
    FiveKind,
}

#[derive(Ord,PartialOrd,Eq,PartialEq,Debug,Clone,Copy)]
struct CardPart1(Card);

#[derive(Eq,PartialEq,Debug,Clone,Copy)]
struct CardPart2(Card);
impl Ord for CardPart2 {
    fn cmp(&self, other: &Self) -> Ordering {
        if self.0 != Card::J && other.0 == Card::J {
            Ordering::Greater
        } else if self.0 == Card::J && other.0 != Card::J {
            Ordering::Less
        } else {
            self.0.cmp(&other.0)
        }
    }
}
impl PartialOrd for CardPart2 {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

#[derive(Debug,Clone)]
struct Hand<T: Ord + Copy>(Vec<T>);

impl<T: Ord + Copy> Hand<T> {
    fn get_type(&self) -> HandType {
        let mut m: BTreeMap<T,usize> = BTreeMap::new();
        for x in &self.0 {
            *m.entry(*x).or_default() += 1;
        }

        if m.iter().filter(|(_,v)| **v == 5).count() >= 1 {
            return HandType::FiveKind;
        }

        if m.iter().filter(|(_,v)| **v == 4).count() >= 1 {
            return HandType::FourKind;
        }

        let mut house = (false,false);
        for (_,v) in &m {
            match v {
                2 => house.0 = true,
                3 => house.1 = true,
                _ => (),
            }
        }
        if house.0 && house.1 {
            return HandType::FullHouse;
        }

        if m.iter().filter(|(_,v)| **v == 3).count() >= 1 {
            return HandType::ThreeKind;
        }

        match m.iter().filter(|(_,v)| **v == 2).count() {
            2 => return HandType::TwoPair,
            1 => return HandType::OnePair,
            _ => (),
        }

        HandType::HighCard
    }
}

impl<T: Ord + Copy> Ord for Hand<T> {
    fn cmp(&self, other: &Self) -> Ordering {
        let (s_t, o_t) = (self.get_type(), other.get_type());
        if s_t < o_t {
            return Ordering::Less;
        }
        if s_t > o_t {
            return Ordering::Greater;
        }

        // check the individual cards in sequence
        for (s,o) in self.0.iter().zip(other.0.iter()) {
            if s == o {
                continue
            }
            return s.cmp(&o);
        }
        Ordering::Equal
    }
}
impl<T: Ord + Copy> PartialEq for Hand<T> {
    fn eq(&self, other: &Self) -> bool {
        self.cmp(other) == Ordering::Equal
    }
}
impl<T: Ord + Copy> Eq for Hand<T>{}
impl<T: Ord + Copy> PartialOrd for Hand<T> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

// just use the "default" cmp stuff
#[derive(Ord,PartialOrd,Eq,PartialEq,Debug,Clone)]
struct HandPart1(Hand<CardPart1>);

// needs special cmp stuff (custom get_type function, rest is equal to Hand.cmp)
#[derive(Eq,PartialEq,Debug,Clone)]
struct HandPart2(Hand<CardPart2>);
impl Ord for HandPart2 {
    fn cmp(&self, other: &Self) -> Ordering {
        let (s_t, o_t) = (self.get_type(), other.get_type());
        if s_t < o_t {
            return Ordering::Less;
        }
        if s_t > o_t {
            return Ordering::Greater;
        }

        // check the individual cards in sequence
        for (s,o) in self.0.0.iter().zip(other.0.0.iter()) {
            if s == o {
                continue
            }
            return s.cmp(&o);
        }
        Ordering::Equal
    }
}
impl PartialOrd for HandPart2 {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl HandPart2 {
    fn get_type(&self) -> HandType {
        let mut m: BTreeMap<CardPart2,usize> = BTreeMap::new();
        for x in &self.0.0 {
            *m.entry(*x).or_default() += 1;
        }

        let joker = *m.get(&CardPart2(Card::J)).unwrap_or(&0);
        let _ = m.remove(&CardPart2(Card::J));

        if m.is_empty() {
            // insert dummy element to still search for stuff even if only jokers were found
            // actually this directly maps to the FiveKind result
            m.insert(CardPart2(Card::A), 0);
        }

        if m.iter().filter(|(_,v)| **v + joker >= 5).count() >= 1 {
            return HandType::FiveKind;
        }

        if m.iter().filter(|(_,v)| **v + joker >= 4).count() >= 1 {
            return HandType::FourKind;
        }

        let mut house = (false,false);
        {
            let mut joker_left = joker;
            for (_,v) in m.iter().rev().take(2) {
                if !house.1 && *v + joker_left >= 3 {
                    joker_left = joker_left.min(*v + joker_left - 3);
                    house.1 = true;
                } else if !house.0 && *v + joker_left >= 2 {
                    joker_left = joker_left.min(*v + joker_left - 2);
                    house.0 = true;
                }
            }
        }
        if house.0 && house.1 {
            return HandType::FullHouse;
        }

        if m.iter().filter(|(_,v)| **v + joker >= 3).count() >= 1 {
            return HandType::ThreeKind;
        }

        // search for pairs
        {
            let mut jokers_left = joker;
            let mut cnt = 0;
            for (_,v) in m.iter().rev() {
                if v + jokers_left >= 2 {
                    jokers_left = v + jokers_left - 2;
                    cnt += 1;
                }
            }
            if cnt >= 2 {
                return HandType::TwoPair
            }
            if cnt >= 1 {
                return HandType::OnePair
            }
        }

        HandType::HighCard
    }
}

#[derive(Debug,Clone)]
pub struct Day07 {
    rounds_1: BTreeMap<HandPart1,u32>,
    rounds_2: BTreeMap<HandPart2,u32>,
}

impl Day07 {
    pub fn new() -> Day07 {
        Day07{
            rounds_1: BTreeMap::new(),
            rounds_2: BTreeMap::new(),
        }
    }
}

impl Day for Day07 {
    fn parse(&mut self) -> Result<()> {
        let file = File::open("./problems/day07.dat").context("reading input file failed")?;
        // let file = File::open("./problems/day07.dat.testing").context("reading input file failed")?;
        let buf = io::BufReader::new(file);

        for line in buf.lines() {
            let line = line?;
            let (h,b) = line.split_once(" ").context("failed to parse round")?;
            let mut hand_1 = Hand(vec![]);
            let mut hand_2 = Hand(vec![]);
            for c in h.chars() {
                hand_1.0.push(CardPart1(c.try_into().context("invalid card")?));
                hand_2.0.push(CardPart2(c.try_into().context("invalid card")?));
            }
            self.rounds_1.insert(HandPart1(hand_1), b.parse().context("invalid bid")?);
            self.rounds_2.insert(HandPart2(hand_2), b.parse().context("invalid bid")?);
        }

        Ok(())
    }

    fn solve_part1(&mut self) -> Result<String> {
        let mut ret = 0;

        for (i,(_,v)) in self.rounds_1.iter().enumerate() {
            ret += (i+1) as u32*v;
        }

        Ok(ret.to_string())
    }

    fn solve_part2(&mut self) -> Result<String> {
        let mut ret = 0;

        for (i,(_,v)) in self.rounds_2.iter().enumerate() {
            ret += (i+1) as u32*v;
        }

        Ok(ret.to_string())
    }
}

impl Default for Day07 {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ordering_1() {
        let hand_1 = Hand(vec![Card::THREE, Card::TWO, Card::T, Card::THREE, Card::K]);
        let hand_2 = Hand(vec![Card::K, Card::K, Card::SIX, Card::SEVEN, Card::SEVEN]);
        let hand_3 = Hand(vec![Card::K, Card::T, Card::J, Card::J, Card::T]);

        assert!(hand_1 < hand_2);
        assert!(hand_1 < hand_3);

        assert!(hand_3 < hand_2);
    }
    #[test]
    fn type_1() {
        let hand_1 = Hand(vec![Card::THREE, Card::TWO, Card::T, Card::THREE, Card::K]);
        let hand_2 = Hand(vec![Card::K, Card::K, Card::SIX, Card::SEVEN, Card::SEVEN]);
        let hand_3 = Hand(vec![Card::K, Card::T, Card::J, Card::J, Card::T]);

        assert_eq!(hand_1.get_type(), HandType::OnePair);
        assert_eq!(hand_2.get_type(), HandType::TwoPair);
        assert_eq!(hand_3.get_type(), HandType::TwoPair);
    }

    #[test]
    fn ordering_2() {
        let hand_1 = HandPart2(Hand(vec![CardPart2(Card::K), CardPart2(Card::T), CardPart2(Card::J), CardPart2(Card::J), CardPart2(Card::T)]));
        let hand_2 = HandPart2(Hand(vec![CardPart2(Card::K), CardPart2(Card::K), CardPart2(Card::A), CardPart2(Card::A), CardPart2(Card::J)]));
        let hand_3 = HandPart2(Hand(vec![CardPart2(Card::J), CardPart2(Card::J), CardPart2(Card::K), CardPart2(Card::SEVEN), CardPart2(Card::A)]));

        assert!(hand_1 > hand_2);
        assert!(hand_1 > hand_3);

        assert!(hand_3 < hand_2);
    }
    #[test]
    fn type_2() {
        let hand_1 = HandPart2(Hand(vec![CardPart2(Card::K),      CardPart2(Card::T),     CardPart2(Card::J),     CardPart2(Card::J),     CardPart2(Card::T)]));
        let hand_2 = HandPart2(Hand(vec![CardPart2(Card::K),      CardPart2(Card::K),     CardPart2(Card::A),     CardPart2(Card::A),     CardPart2(Card::J)]));
        let hand_3 = HandPart2(Hand(vec![CardPart2(Card::J),      CardPart2(Card::J),     CardPart2(Card::K),     CardPart2(Card::SEVEN), CardPart2(Card::A)]));

        let hand_04 = HandPart2(Hand(vec![CardPart2(Card::A),     CardPart2(Card::K),     CardPart2(Card::K),     CardPart2(Card::Q),     CardPart2(Card::K)]));
        let hand_05 = HandPart2(Hand(vec![CardPart2(Card::K),     CardPart2(Card::THREE), CardPart2(Card::K),     CardPart2(Card::FIVE),  CardPart2(Card::TWO)]));
        let hand_06 = HandPart2(Hand(vec![CardPart2(Card::SEVEN), CardPart2(Card::FIVE),  CardPart2(Card::J),     CardPart2(Card::THREE), CardPart2(Card::TWO)]));
        let hand_07 = HandPart2(Hand(vec![CardPart2(Card::FOUR),  CardPart2(Card::J),     CardPart2(Card::THREE), CardPart2(Card::FOUR),  CardPart2(Card::THREE)]));
        let hand_08 = HandPart2(Hand(vec![CardPart2(Card::TWO),   CardPart2(Card::SEVEN), CardPart2(Card::Q),     CardPart2(Card::TWO),   CardPart2(Card::NINE)]));
        let hand_09 = HandPart2(Hand(vec![CardPart2(Card::A),     CardPart2(Card::FOUR),  CardPart2(Card::J),     CardPart2(Card::TWO),   CardPart2(Card::FIVE)]));
        let hand_10 = HandPart2(Hand(vec![CardPart2(Card::NINE),  CardPart2(Card::Q),     CardPart2(Card::THREE), CardPart2(Card::FOUR),  CardPart2(Card::SEVEN)]));
        let hand_11 = HandPart2(Hand(vec![CardPart2(Card::T),     CardPart2(Card::Q),     CardPart2(Card::SIX),   CardPart2(Card::A),     CardPart2(Card::Q)]));
        let hand_12 = HandPart2(Hand(vec![CardPart2(Card::FOUR),  CardPart2(Card::THREE), CardPart2(Card::Q),     CardPart2(Card::SEVEN), CardPart2(Card::THREE)]));
        let hand_13 = HandPart2(Hand(vec![CardPart2(Card::A),     CardPart2(Card::A),     CardPart2(Card::NINE),  CardPart2(Card::FOUR),  CardPart2(Card::SIX)]));
        let hand_14 = HandPart2(Hand(vec![CardPart2(Card::J),     CardPart2(Card::J),     CardPart2(Card::J),  CardPart2(Card::J),  CardPart2(Card::J)]));

        assert_eq!(hand_1.get_type(), HandType::FourKind, "{:?}", hand_1);
        assert_eq!(hand_2.get_type(), HandType::FullHouse, "{:?}", hand_2);
        assert_eq!(hand_3.get_type(), HandType::ThreeKind, "{:?}", hand_3);

        assert_eq!(hand_04.get_type(), HandType::ThreeKind, "{:?}", hand_04);
        assert_eq!(hand_05.get_type(), HandType::OnePair, "{:?}", hand_05);
        assert_eq!(hand_06.get_type(), HandType::OnePair, "{:?}", hand_06); // TODO
        assert_eq!(hand_07.get_type(), HandType::FullHouse, "{:?}", hand_07);
        assert_eq!(hand_08.get_type(), HandType::OnePair, "{:?}", hand_08);
        assert_eq!(hand_09.get_type(), HandType::OnePair, "{:?}", hand_09);
        assert_eq!(hand_10.get_type(), HandType::HighCard, "{:?}", hand_10);
        assert_eq!(hand_11.get_type(), HandType::OnePair, "{:?}", hand_11);
        assert_eq!(hand_12.get_type(), HandType::OnePair, "{:?}", hand_12);
        assert_eq!(hand_13.get_type(), HandType::OnePair, "{:?}", hand_13);
        assert_eq!(hand_14.get_type(), HandType::FiveKind, "{:?}", hand_14);
    }
}
