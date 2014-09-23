function c_test()
	local pack = A()
	pack:show()
	print("get:", pack:get())
	print("set:", pack:set(11))
	
	local pack = WorldPacket(23, 20)
	print(pack:GetOpcode())	
	print(pack:GetSize())
	pack:WriteUShort(10)
	pack:WriteUShort(11)
	pack:WriteUShort(12)
	print(pack:GetSize())
end

function c_testInt()
	local val = 1314
	return val
end

function c_testStr()
	local str = "hello world"
	return str
end

