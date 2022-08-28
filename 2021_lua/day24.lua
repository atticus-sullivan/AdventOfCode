local utils = require"aocutils"

local _M = {}

local function round_to_zero(num) 
    if num >= 0 then return math.floor(num) 
    else return math.ceil(num) end
end

local function parse_file(file)
	local ret = {}
	local r = {}
	for line in file:lines() do
		local i
		local op,a,b = line:match("(.*) (.*) (.*)")
		if not op then
			-- b stays nil
			op,a = line:match("(inp) (.*)")
			if #r > 0 then
				table.insert(ret, r)
				r = {}
			end
		end
		local b_ = tonumber(b)
		if b_ then b = b_ end
		table.insert(r, {op=op, a=a, b=b})
	end
	table.insert(ret, r)
	return ret
end

-- FIXME: Pretty ugly code with masice code duplication (part lazieness, part
-- trying to gain performance by eliminating ifs) Would be possible to make
-- much more performant by solving by inspection (looking at the input just
-- with pen and paper), maybe a better performance can be achieved by changing
-- some loop nesting orders
-- for a pretty amazing solution look at
-- https://www.reddit.com/r/adventofcode/comments/rnj7r7/comment/hqhjqzu/?utm_source=share&utm_medium=web2x&context=3
-- and
-- https://topaz.github.io/paste/#XQAAAQA8BAAAAAAAAAAyGUj/T3PF3S7nOSzpbtFoVYGExYCpdz/gDu7vwznxV2ytKQn0CpZ2P3jgKRS424E8Ai4fT6Z4/l4bQwRch1Su4LF2iFEgsDAHscTQL6L7FskVoR45ICc5+k7e6MAXGAyErgVXBNH7sB2Yd0493ZtVkg17wKox71bX1dGJo/jRiPtmSdN99Hu912N7fAHjsa6wjfKzx9qpsGl5AY0uc/8Nl/uaGTJ1ROrwESX7w0QjD9NiYxUwzmMgQgqYLZufxDroqJT8pysN2Zn8vq/ZW6mgdc8DhDF7Q8TotEGEB8pCDJPhBFBUdO/jqgoI+r+HY3Leqgi5W6bb+7Sm8goNYf5VUidTcKSMdmgqucmUiKxIjaFSW8iKhdWDn0kx8Iouoj0m93DBJLYNYoM6Pa3LdX3Rendtj/LPZovAKryVSgdZt/W0knrmRNZ46UKjmgSTICVM/hZkTB6BQspJSmwava/9SyWxldEibRy+G0nYFEQioHQolaEAJKJbgEQygOcSz7xNmluTNZ+9YoE8oJNK0TpDNexR8ZK+uO5fZhlZEcteiVfv3qDwYCyZPwtnTeuryUuYMTO3VJahoDBsNrbCN9B9c8V/54+ehhaJZmiwrc6/RgNZDYUy81683JGfQ5xsL3K078tDkDRnRnl2Kug+wJKyqTjVZ24Fi/08Jpb3yFWvJ8UyCVq8IKMX66b8Gp/i
-- note that these solutions are really fast but also pretty specific to the structure of the input

-- turns out that the code is similar to using a stack where some digit must match another digit (with an offset)

local exec_instr = {
	["inp"] = function(c,i) c[i.a] = c.digit end,
	["add"] = function(c,i) c[i.a] = c[i.a] + (type(i.b) == "number" and i.b or c[i.b]) end,
	["mul"] = function(c,i) c[i.a] = c[i.a] * (type(i.b) == "number" and i.b or c[i.b]) end,
	["mod"] = function(c,i) c[i.a] = c[i.a] % (type(i.b) == "number" and i.b or c[i.b]) end,
	["div"] = function(c,i) c[i.a] =
		round_to_zero(c[i.a] / (type(i.b) == "number" and i.b or c[i.b]))
	end,
	["eql"] = function(c,i)
		if c[i.a] == (type(i.b) == "number" and i.b or c[i.b]) then
			c[i.a] = 1
		else
			c[i.a] = 0
		end
	end,
}

local function exec_digit(configs, digit, instructions, new_configs)
	for z,input in pairs(configs) do
		local c = {w=0, x=0, y=0, z=z, digit=digit}
		for _,instr in ipairs(instructions) do
			exec_instr[instr.op](c, instr)
		end
			if new_configs[c.z] then
				new_configs[c.z] = math.max(new_configs[c.z], input*10 + c.digit)
			else
				new_configs[c.z] = input*10 + c.digit
			end
	end
end

function _M.part1(file)
	local code = parse_file(file)
	local configs = {[0]=0}
	for _,instruction_set in ipairs(code) do
		local new_configs = {}
		for d=1,9 do
			exec_digit(configs, d, instruction_set, new_configs)
		end
		configs = new_configs
	end
	return configs[0]
end

local function exec_digit2(configs, digit, instructions, new_configs)
	for z,input in pairs(configs) do
		local c = {w=0, x=0, y=0, z=z, digit=digit}
		for _,instr in ipairs(instructions) do
			exec_instr[instr.op](c, instr)
		end
			if new_configs[c.z] then
				new_configs[c.z] = math.min(new_configs[c.z], input*10 + c.digit)
			else
				new_configs[c.z] = input*10 + c.digit
			end
	end
end

function _M.part2(file)
	local code = parse_file(file)
	local configs = {[0]=0}
	for _,instruction_set in ipairs(code) do
		local new_configs = {}
		for d=1,9 do
			exec_digit2(configs, d, instruction_set, new_configs)
		end
		configs = new_configs
	end
	return configs[0]
end

return _M
