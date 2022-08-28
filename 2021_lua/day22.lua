local utils = require"aocutils"

local _M = {}

function math.sign(x)
	if x < 0 then
		return -1
	elseif x == 0 then
		return 0
	else
		return 1
	end
end

local function parse_file(file)
	local r = {}
	for line in file:lines() do
		local op,minX,maxX,minY,maxY,minZ,maxZ = line:match("^(.+) x=(%-?%d+)%.%.(%-?%d+),y=(%-?%d+)%.%.(%-?%d+),z=(%-?%d+)%.%.(%-?%d+)$")
		minX,maxX, minY,maxY, minZ,maxZ = tonumber(minX),tonumber(maxX), tonumber(minY),tonumber(maxY), tonumber(minZ),tonumber(maxZ)
		table.insert(r, {
			op=op, volume=(maxX-minX+1)*(maxY-minY+1)*(maxZ-minZ+1)*(op=="on" and 1 or -1),
			minX=minX,maxX=maxX, minY=minY,maxY=maxY, minZ=minZ,maxZ=maxZ
		})
	end
	return r
end

local function solve(instructions)
	local cuboids = {}
	local function cnt()
		local r = 0
		for _,c in ipairs(cuboids) do
			r = r + c.volume
		end
		return r
	end
	local function inter(c1,c2)
		if
			(c1.minX <= c2.maxX and c1.maxX >= c2.minX) and
			(c1.minY <= c2.maxY and c1.maxY >= c2.minY) and
			(c1.minZ <= c2.maxZ and c1.maxZ >= c2.minZ) then
			local minX,maxX, minY,maxY, minZ,maxZ =
				math.max(c1.minX,c2.minX),math.min(c1.maxX,c2.maxX),
				math.max(c1.minY,c2.minY),math.min(c1.maxY,c2.maxY),
				math.max(c1.minZ,c2.minZ),math.min(c1.maxZ,c2.maxZ)
			return {
				volume=(maxX-minX+1)*(maxY-minY+1)*(maxZ-minZ+1)*math.sign(c1.volume)*-1, -- opposite of c1
				minX=minX,maxX=maxX, minY=minY,maxY=maxY, minZ=minZ,maxZ=maxZ
			}
		else
			return nil
		end
	end
	local function add_cube(cuboid)
		for ci=1,#cuboids do
			local i = inter(cuboids[ci], cuboid)
			if i then
				-- print(i.volume, i.minX,i.maxX, i.minY,i.maxY, i.minZ,i.maxZ)
				table.insert(cuboids, i)
			end
		end
		if cuboid.volume > 0 then
			-- print(cuboid.volume, cuboid.minX,cuboid.maxX, cuboid.minY,cuboid.maxY, cuboid.minZ,cuboid.maxZ)
			table.insert(cuboids, cuboid)
		end
		-- print()
	end

	for _,instr in ipairs(instructions) do
		add_cube(instr)
	end

	return cnt()
end

function _M.part1(file)
	-- file = io.open("day22.dat.testing2")
	local instructions = parse_file(file)
	local valid = {}
	for _,instr in ipairs(instructions) do
		if instr.minX < -50 and instr.maxX > -50 then
			instr.minX = -50
		end
		if instr.minY < -50 and instr.maxY > -50 then
			instr.minY = -50
		end
		if instr.minZ < -50 and instr.maxZ > -50 then
			instr.minZ = -50
		end

		if instr.maxX > 50 and instr.minX < 50 then
			instr.maxX = 50
		end
		if instr.maxY > 50 and instr.minY < 50 then
			instr.maxY = 50
		end
		if instr.maxZ > 50 and instr.minZ < 50 then
			instr.maxZ = 50
		end
		if
			-50 <= instr.minX and instr.maxX <= 50 and
			-50 <= instr.minY and instr.maxY <= 50 and
			-50 <= instr.minZ and instr.maxZ <= 50
			then
				table.insert(valid, instr)
			end
	end
	return solve(valid)
end

function _M.part2(file)
	-- file = io.open("day22.dat.testing3")
	local instructions = parse_file(file)
	return solve(instructions)
end

return _M
