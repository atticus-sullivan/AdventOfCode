-- Advent Of Code 2021
-- Copyright (C) 2024  Lukas Heindl
-- 
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

local term   = require"term"
local colors = require"term.colors"

-- see https://github.com/sumneko/lua-language-server/wiki/EmmyLua-Annotations#class

-- global helper functions
local _M = {}

-- functions to work on classic tables
_M.table = {}
-- returns a shallow copy of the given table
---@param t table table to copy
---@return table res shallow copy
function _M.table.copy_shallow(t)
	local res = {}
	for i,v in pairs(t) do
		res[i] = v
	end
	return res
end
-- returns a deep copy of the given table (no metatable copying)
---@param t table table to copy
---@return table res deep copy
function _M.table.copy_deep(t)
	if type(t) ~= "table" then return t end
	local res = {}
	for i,v in pairs(t) do
		res[i] = _M.table.copy_deep(v)
	end
	return res
end
-- deep prints the table completly with keys and values
---@param t table table to print
---@param shift number indentation of the levels
function _M.table.print(t, shift)
	local function printTable(t, indent)
		if type(t) ~= "table" then io.write(string.rep(" ", indent), tostring(t), "\n") return end
		for k,v in pairs(t) do
			io.write(string.rep(" ", indent), k, " -> \n")
			printTable(v, indent+(shift or 2))
		end
	end
	printTable(t, 0)
end

function _M.table.serialize(t)
	if type(t) ~= "table" then return tostring(t) end

	local r = {}
	for k,v in pairs(t) do
		table.insert(r, string.format("%s={%s},", k, _M.table.serialize(v)))
	end
	return table.concat(r)
end

-- functions to work on sets (values stored in the keys)
_M.set = {}
-- inserts  an element into the set
---@param set table set to work on
---@param ele any element to insert (has to be hashable -> no tables)
---@return nil|any state `nil` if the element already was contained, `ele` on succ
function _M.set.insert(set, ele)
	if set[ele] ~= nil then return nil end
	set[ele] = true
	return ele
end
-- inserts  an element into the set (doesn't check if elemnt was contained)
---@param set table set to work on
---@param ele any element to remove (has to be hashable -> no tables)
---@return any ele the element which was to be removed
function _M.set.remove(set, ele)
	set[ele] = nil
	return ele
end
-- performs an union of the given sets and returns a set containing the union
---@param a table set
---@param b table set
---@return table union set containing the union of `a` and `b`
function _M.set.union(a, b)
	local ret = {}
	for k,_ in pairs(a) do
		ret[k] = true
	end
	for k,_ in pairs(b) do
		ret[k] = true
	end
	return ret
end
-- performs an intersection of the given sets and returns a set containing the intersection
---@param a table set
---@param b table set
---@return table intersection set containing the intersection of `a` and `b`
function _M.set.intersect(a, b)
	local ret = {}
	for k,_ in pairs(a) do
		if b[k] == true and a[k] == true then
			ret[k] = true
		else
			ret[k] = false
		end
	end
	return ret
end
-- get the length of the given set
---@param s table set to work on
---@return number length length of the set
function _M.set.len(s)
	local len = 0
	for _,v in pairs(s) do
		if v then len = len + 1 end
	end
	return len
end
-- convert a set to an array
---@param s table set to work on
---@param a table array to append to (optional)
---@return table array with the set at its end
function _M.set.to_arr(s, a)
	a = a or {}
	for k,_ in pairs(s) do
		table.insert(a, k)
	end
	return a
end
-- remove and return a random set entry
---@param s table set to work on
---@return any random popped set value
function _M.set.pop_rand(s)
	for v,_ in pairs(s) do
		s[v] = nil
		return v
	end
	return nil
end
-- prints the set
---@param s table set to print
---@param pr fun(x:any): printable function to print the elements of the set
function _M.set.print(s, pr)
	pr = pr or function(x) return x end
	for k,_ in pairs(s) do
		io.write(pr(k))
		io.write(", ")
	end
	print()
end

---@class pq
---@field q table
---@field min number
-- functions to work on a priority queue:table{q:table[eles], min:number}
_M.pq = {}
-- checks if the queue is empty
---@param pq pq pq to work on
--- returns boolean pq.is_empty()
function _M.pq.empty(pq)
	return not pq.min or not pq.q[pq.min] or #pq.q[pq.min] <= 0
end
-- inserts ele with key into the queue
---@param pq pq pq to work on
---@param ele table element to insert into the pq
---@param key number key of the element (stored in ele.key)
function _M.pq.insert(pq, ele, key)
	ele.key = key
	if not pq.q[key] then
		pq.q[key] = {}
	end
	table.insert(pq.q[key], ele)
	if not pq.min or key < pq.min then pq.min = key end
end
-- updates the key of the element
---@param pq pq pq to work on
---@param ele table element to update
---@param key number new key of the element (stored in ele.key)
function _M.pq.decKey(pq, ele, key)
	local ind = 0
	for i,v in ipairs(pq.q[ele.key]) do
		if v == ele then
			ind = i
			break
		end
	end
	table.remove(pq.q[ele.key], ind)
	if #pq.q[ele.key] <= 0 then
		pq.q[ele.key] = nil
		pq.min = nil
		for k,_ in pairs(pq.q) do
			if not pq.min or k < pq.min then pq.min = k end
		end
	end
	_M.pq.insert(pq, ele, key)
end
-- search and remove the element with the minimal key
---@param pq pq pq to work on
---@return table|nil ele the ele with the lowest key, `nil` if pq ia already empty
function _M.pq.delmin(pq)
	if _M.pq.empty(pq) then return nil end
	local r = table.remove(pq.q[pq.min])
	if #pq.q[pq.min] <= 0 then
		pq.q[pq.min] = nil

		pq.min = nil
		for k,_ in pairs(pq.q) do
			if not pq.min or k < pq.min then pq.min = k end
		end
	end
	return r
end
-- prints the queue
---@param pq pq pq to print
---@param pr fun(x:any): printable function to print the elements of the pq
function _M.pq.print(pq, pr)
	pr = pr or function(x) return x end
	for k,v in pairs(pq.q) do
		print(k)
		for _,x in ipairs(v) do
			io.write("    ", pr(x))
		end
	end
end

---@class board
---@field board table<string, any>
---@field mima table minx:number, maxx:number, miny:number, maxy:number
-- functions for a board: {board: table<string, any>, mima: table{minx:number, maxx:number, miny:number, maxy:number}}
-- mima is only stored for the printing
_M.board = {}
-- set a field in the board to a value
---@param b board board to work on
---@param x number coordinate
---@param y number coordinate
---@param val any value to insert
---@param mima boolean update mima field with this (should be `true` if a new value is being set)
function _M.board.set(b, x, y, val, mima)
	local c = string.format("%d,%d", x, y)
	b.board[c] = val

	if mima then
		if x < b.mima.minx then b.mima.minx = x end
		if x > b.mima.maxx then b.mima.maxx = x end

		if y < b.mima.miny then b.mima.miny = y end
		if y > b.mima.maxy then b.mima.maxy = y end
	end
end
-- update the mima field
---@param b board board to work on
---@param mima table if `minx`,`maxx`,`miny` and/or `maxy` are set the corresponding field is being updated
function _M.board.update_mima(b, mima)
	if mima.minx then b.mima.minx = mima.minx end
	if mima.maxx then b.mima.maxx = mima.maxx end

	if mima.miny then b.mima.miny = mima.miny end
	if mima.maxy then b.mima.maxy = mima.maxy end
end
-- get the ele on the coordinates
---@param b board board to work on
---@param x number coordinate
---@param y number coordinate
---@return any|nil element on given position, `nil` if field isn't set
function _M.board.get(b, x, y)
	local c = string.format("%d,%d", x, y)
	return b.board[c]
end
-- check if coordinate is inside mima range
---@param b board board to work on
---@param x number coordinate
---@param y number coordinate
---@return boolean if coordinate is inside of mima range
function _M.board.in_range(b, x, y)
	return b.mima.minx <= x and x <= b.mima.maxx and
		b.mima.miny <= y and y <= b.mima.maxy
end
-- find/update mima field (Attention: Loops through the whole board)
---@param b board board to work on
function _M.board.find_mima(b)
	b.mima = {}
	for k,_ in pairs(b.board) do
		local x,y = k:match("(%-?%d+),(%-?%d+)")
		if not b.mima.minx or x < b.mima.minx then b.mima.minx = x end
		if not b.mima.maxx or x > b.mima.maxx then b.mima.maxx = x end

		if not b.mima.miny or y < b.mima.miny then b.mima.miny = y end
		if not b.mima.maxy or y > b.mima.maxy then b.mima.maxy = y end
	end
end
-- prints the board
---@param b board board to print
---@param graphic table {bool:boolean graphic printing?, pre:string print before, pred:fun(x:ele):boolean highlight ele on printing}
---@param pr fun(x:any): printable function to print the elements of the board
function _M.board.print(b, pr, graphic)
	pr = pr or function(x) return x end
	graphic = graphic or {pre="", pred=function(x) return x end}
	if graphic.bool then
		term.clear()
		term.cursor['goto'](1, 1)
		if graphic.pre then print(graphic.pre) end
	end
	for y = b.mima.miny, b.mima.maxy do
		for x = b.mima.minx, b.mima.maxx do
			local c = string.format("%d,%d", x,y)
			if graphic.pred(b.board[c]) and graphic.bool then
				io.write(colors.red(pr(b.board[c])))
			else
				io.write(pr(b.board[c]))
			end
		end
		io.write("\n")
	end
end

function _M.board.count(b, pred)
	local r = 0
	for y = b.mima.miny, b.mima.maxy do
		for x = b.mima.minx, b.mima.maxx do
			local c = string.format("%d,%d", x,y)
			if pred(b.board[c]) then r = r + 1 end
		end
	end
	return r
end

return _M
