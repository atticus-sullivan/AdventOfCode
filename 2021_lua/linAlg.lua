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

local _M = {}

-- A[line][column]

_M.matrix = {}
function _M.matrix.print(x, n)
	if n then
		io.write(n, ":\n")
	end
	for _,l in ipairs(x) do
		for _,e in ipairs(l) do
			io.write(e, " ")
		end
		io.write("\n")
	end
end

_M.vector = {}
function _M.vector.print(x, n)
	if n then
		io.write(n, ":\n")
	end
	for _,e in ipairs(x) do
		io.write(e, " ")
	end
	io.write("\n")
end

_M.solver = {}
function _M.solver.lu_factorization3(a)
	assert(#a == 3, "lu_factorization only implemented for dim = 3")
	local l,r = {{}, {}, {}}, {{}, {}, {}}

	r[1][1], r[1][2], r[1][3] = a[1][1], a[1][2], a[1][3]
	l[1][1], l[1][2], l[1][3] = 1, 0, 0

	l[2][1], l[2][2], l[2][3] = a[2][1]/r[1][1], 1, 0
	r[2][1], r[2][2], r[2][3] = 0, a[2][2] - l[2][1]*r[1][2], a[2][3] - l[2][1]*r[1][3]

	l[3][1] = a[3][1]/r[1][1]
	l[3][2], l[3][3] = (a[3][2] - l[3][1]*r[1][2])/r[2][2], 1

	r[3][1], r[3][2], r[3][3] = 0, 0, a[3][3] - l[3][1]*r[1][3] - l[3][2]*r[2][3]

	return l,r
end
function _M.solver.lu_factorization(a)
	local l,r = {}, {}
	for i=1,#a do
		l[i],r[i] = {},{}
	end
	-- populate the fixed values of l/r
	for i = 1, #a do
		for j = 1, #a do
			if i == j    then l[i][j] = 1
			elseif j > i then l[i][j] = 0
			end
			if j < i then r[i][j] = 0
			end
		end
	end

	-- calculate remaining values
	for i = 1, #a do
		-- populate ith line of l
		for j = 1, #a do
			if j < i then
				l[i][j] = a[i][j]
				for k = 1, j-1 do
					l[i][j] = l[i][j] - l[i][k]*r[k][j]
				end
				l[i][j] = l[i][j] / r[j][j]
			end
		end
		-- populate ith line of k
		for j = 1, #a do
			if j >= i then
				r[i][j] = a[i][j]
				for k = 1, i-1 do
					r[i][j] = r[i][j] - l[i][k]*r[k][j]
				end
				r[i][j] = r[i][j] / l[j][j]
			end
		end
	end

	return l,r
end
function _M.solver.subst_back(a, b)
	local x = {}
	for i = #a, 1, -1 do
		x[i] = b[i]
		for j = i+1, #a do
			x[i] = x[i] - a[i][j] * x[j]
		end
		x[i] = x[i] / a[i][i]
	end
	return x
end
function _M.solver.subst_forward(a, b)
	local x = {}
	for i = 1, #a do
		x[i] = b[i]
		for j = 1, i-1 do
			x[i] = x[i] - a[i][j] * x[j]
		end
		x[i] = x[i] / a[i][i]
	end
	return x
end
function _M.solver.solve_lu(a, b, l, u)
	if not l or not u then
		l,u = _M.solver.lu_factorization(a)
	end
	local y   = _M.solver.subst_forward(l, b)
	return      _M.solver.subst_back(u, y), l, u
end
-- optional: use memoization instead of letting the user do it manually.
-- PROBLEM: How to check if same matrix is given -> O(n^2) comparisons
-- PROBLEM: How to handle memory? Always copy? -> expensive too
-- local function memoize(fn)
--     fn = fn or function(_) return nil end
--     return setmetatable({},
--       {
--        __index = function(t, k)
--                    local val = fn(k)
--                    t[k] = val
--                    return val
--                  end,
--        __call  = function(t, k)
--                    return t[k]
--                  end
--       })
-- end

local function main3()
	local a = {
		{-618 , -824 , -621 , 1},
		{-537 , -823 , -458 , 1},
		{-447 , -329 ,  318 , 1},
		{ 404 , -588 , -901 , 1},
	}

	local b
	b = {686, 605, 515, -336}

	_M.matrix.print(a, "A")
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")

	b = {917,935,430,889}
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")

	b = {-361,-424,532,-390}
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")
end
-- main3()

local function main2()
	local a = {
		{-447,-329,318,1},
		{-345,-311,381,1},
		{-661,-816,575,1},
		{-485,-357,347,1},
	}

	local b
	b = {515, 413, 729, 553}

	_M.matrix.print(a, "A")
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")

	b = {917,935,430,889}
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")

	b = {-361,-424,532,-390}
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")
end
-- main2()

local function main()
	local a = {
		{4,2,3},
		{2,2,1},
		{2,2,2},
	}

	local b = {5, -3, 0}

	_M.matrix.print(a, "A")
	print()
	_M.vector.print(_M.solver.solve_lu(a, b), "res")
end
-- main()

return _M
