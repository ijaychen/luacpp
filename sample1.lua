function c_test()
	print(A)
	local pack = A()
	pack:show()
	print("get:", pack:get())
--	print("getName:", pack:getName())
	print("set:", pack:set(11))
end

function c_testInt()
	local val = 1314
	return val
end

function c_testStr()
	local str = "hello world"
	return str
end

