local _M = {}

local function parse_file(file)
	local minx,maxx, miny,maxy = file:read("l"):match("target area: x=([%-]?%d+)%.%.([%-]?%d+), y=([%-]?%d+)%.%.([%-]?%d+)")
	return {minx=tonumber(minx),maxx=tonumber(maxx), miny=tonumber(miny),maxy=tonumber(maxy)}
end

local function hits(target, pos, v)
	local function is_in(target, pos)
		return target.minx <= pos.x and pos.x <= target.maxx and target.miny <= pos.y and pos.y <= target.maxy
	end
	local function is_over(target, pos)
		return pos.y < target.miny
	end

	local max = 0
	while not is_over(target, pos) do
		pos.x = pos.x + v.x
		pos.y = pos.y + v.y

		if pos.y > max then max = pos.y end

		if is_in(target, pos) then return true,max end

		if v.x < 0 then
			v.x = v.x +1
		elseif v.x > 0 then
			v.x = v.x -1
		end

		v.y = v.y -1
	end
	return false, nil
end

-- reading
--     https://www.reddit.com/r/adventofcode/comments/ri9kdq/comment/hpkn6za/
-- it it possible to work this out much faster. But then as noted in
--     https://www.reddit.com/r/adventofcode/comments/ri9kdq/comment/hsunes7/
-- we cannot be sure if there is an v.x which hits the target
-- (but just checking the calculated maximum and then going down would be much faster than going upwards)
function _M.part1(file)
	file = io.open("./day17.dat")
	local target = parse_file(file)
	local y = 1
	local stats = {miss=0, max=0, v={}}
	while stats.miss < 10000 do
		for x=1,target.maxx+1 do
			local v = {x=x, y=y}
			local h,m = hits(target, {x=0,y=0}, v)
			if h then
				stats.miss = 0
				stats.max  = m
				stats.v    = v
				break
			else
				stats.miss = stats.miss+1
			end
		end
		y = y + 1
	end
	return stats.max
end

-- reading
--     https://www.reddit.com/r/adventofcode/comments/ri9kdq/2021_day_17_solutions/
-- there are some more heuristics which could be applied which v s to check, but I see 
-- no real clean solution
-- idea from
--     https://github.com/Tristan-H11/AdventOfCode21/blob/master/src/main/scala/Day17/Day17.scala
-- use yVelocity <- yRange.min to 1000
-- (start with yMin and then go up)
function _M.part2(file)
	file = io.open("./day17.dat")
	local target = parse_file(file)
	local y = 1
	local stats = {miss=0, cnt=0}
	while stats.miss < 10000 do
		for x=1,target.maxx+1 do
			local v = {x=x, y=y}
			local h,m = hits(target, {x=0,y=0}, v)
			if h then
				-- print(x, y)
				stats.miss = 0
				stats.cnt  = stats.cnt + 1
			else
				stats.miss = stats.miss+1
			end
		end
		y = y + 1
	end
	y = 0
	stats.miss = 0
	while stats.miss < 10000 do
		for x=1,target.maxx+1 do
			local v = {x=x, y=y}
			local h,m = hits(target, {x=0,y=0}, v)
			if h then
				-- print(x, y)
				stats.miss = 0
				stats.cnt  = stats.cnt + 1
			else
				stats.miss = stats.miss+1
			end
		end
		y = y - 1
	end
	return stats.cnt
end

return _M
