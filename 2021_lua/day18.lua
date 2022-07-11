local utils = require("aocutils")

local _M = {}

Pair = {}
function Pair:new(o) -- key/value passing
	if getmetatable(o) == Pair then
		o = {left=o.left, right=o.right}
	end
	o = o or {left=nil, right=nil}
	setmetatable(o, self) -- use the same metatable for all objects
	self.__index = self -- lookup in class if key is not found
	return o
end
function Pair:clone()
	return Pair:new{
		left  = type(self.left) == "table" and self.left:clone() or self.left,
		right = type(self.right) == "table" and self.right:clone() or self.right,
	}
end
function Pair.parse(s)
	s = s:sub(2, -2) -- remove first and last character (unpacking)

	-- parse left side
	local l,left = s:match("^%b[]")
	if l then
		-- left side is a pair
		left = Pair.parse(l)
		-- remove parsed stuff
		s = s:gsub("^%b[],", "")
	else
		-- left side is a number
		left = tonumber(s:match("^%d+"))
		-- remove parsed stuff
		s = s:gsub("^%d+,", "")
	end

	local r,right = s:match("^%b[]")
	if r then
		-- right side is a pair
		right = Pair.parse(r)
		-- remove parsed stuff
		s = s:gsub("^%b[],", "")
	else
		-- right side is a number
		right = tonumber(s:match("^%d+"))
		-- remove parsed stuff
		s = s:gsub("^%d+,", "")
	end

	return Pair:new{left=left, right=right}
end
function Pair:print(nl)
	nl = nl == nil and true or false
	io.write("[")
	if type(self.left) == "table" then
		self.left:print(false)
	else
		io.write(self.left)
	end
	io.write(",")
	if type(self.right) == "table" then
		self.right:print(false)
	else
		io.write(self.right)
	end
	io.write("]")
	if nl then
		print()
	end
end
function Pair:explode(depth)
	depth = depth or 0 -- default

	if depth == 4 then
		-- do explosion
		return self.left, 0, self.right, true
	end

	local l,r
	if type(self.left) == "table" then
		local e
		l,self.left,r,e = self.left:explode(depth+1)
		if r then
			if type(self.right) == "table" then
				self.right:add_left(r)
			else
				self.right = self.right +r
			end
		end
		if e then
			return l,self,nil, true
		end
	end

	if type(self.right) == "table" and not e then
		local e
		l,self.right,r,e = self.right:explode(depth+1)
		if l then
			if type(self.left) == "table" then
				self.left:add_right(l)
			else
				self.left = self.left +l
			end
		end
		if e then
			return nil,self,r, true
		end
	end
	return nil,self,nil, false
end
function Pair:add_left(x)
	if type(self.left) == "table" then self.left:add_left(x)
	else
		self.left = self.left +x
	end
end
function Pair:add_right(x)
	if type(self.right) == "table" then self.right:add_right(x)
	else
		self.right = self.right +x
	end
end
function Pair:split()
	if type(self.left) == "table" then
		if self.left:split() then
			return true
		end
	elseif self.left >= 10 then
		self.left = Pair:new{left=self.left//2, right=math.ceil(self.left/2)}
		return true
	end

	if type(self.right) == "table" then
		if self.right:split() then
			return true
		end
	elseif self.right >= 10 then
		self.right = Pair:new{left=self.right//2, right=math.ceil(self.right/2)}
		return true
	end
	return false
end
function Pair:reduce()
	local r = true
	while r do
		-- explode all
		while r do
			_,_,_,r = self:explode()
		end
		r = self:split()
	end
	return self -- allow chaining
end
function Pair.__add(lhs, rhs)
	assert(getmetatable(lhs) == getmetatable(rhs) and getmetatable(rhs) == Pair, "add is only defined on pairs")
	return Pair:new{left=lhs, right=rhs}:reduce()
end
function Pair:magnitude()
	local magn = 0
	if type(self.left) == "table" then
		magn = magn + 3*self.left:magnitude()
	else
		magn = magn + 3*self.left
	end
	if type(self.right) == "table" then
		magn = magn + 2*self.right:magnitude()
	else
		magn = magn + 2*self.right
	end
	return magn
end


function _M.part1(file)
	-- file = io.open("./day18.dat.testing")
	local p
	local ln = file:read("l")
	p = Pair.parse(ln)
	for line in file:lines() do
		p = p + Pair.parse(line)
	end
	return p:magnitude()
end

function _M.part2(file)
	-- file = io.open("./day18.dat.testing")
	local ps, m = {}, 0
	for line in file:lines() do
		local p = Pair.parse(line)
		table.insert(ps, p)
	end
	-- no neat triangular loop, because of non commutativity
	for _,p1 in ipairs(ps) do
		for _,p2 in ipairs(ps) do
			local _m = (p1:clone() + p2:clone()):magnitude()
			if _m > m then
				m = _m
			end
		end
	end
	return m
end

return _M
