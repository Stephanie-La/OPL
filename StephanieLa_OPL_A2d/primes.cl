read n
cp := 2
while n > 0
	found := 0
	cf1 := 2
	cf1s := cf1 * cf1 
	while cf1s <= cp
		cf2 := 2
		pr := cf1 * cf2
		while pr <= cp
			if pr = cp
				found := 1
			end
			cf2 := cf2 + 1
			pr := cf1 * cf2
		end
		cf1 := cf1 + 1
		cf1s := cf1 * cf1
	end
	if found = 0
		write cp
		n := n - 1
	end
	cp := cp + 1
end
