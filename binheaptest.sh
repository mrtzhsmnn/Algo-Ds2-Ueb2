g++ -o binheaptest binheaptest.cxx

cat <<- END > binheaptest.in
	+ a
	+ b
	+ c
	+ d
	+ e
	+ f
	+ g
	+ h
	+ i
	?
	= 7 c
	!
	- 2
	+ j
	= 7 k
	q
	END

./binheaptest echo < binheaptest.in > binheaptest.out

cat <<- END > binheaptest.exp
	0 entry(s)
	cmd: + a
	a 0
	1 entry(s)
	cmd: + b
	a 0
	  b 1
	2 entry(s)
	cmd: + c
	c 2
	a 0
	  b 1
	3 entry(s)
	cmd: + d
	a 0
	  b 1
	  c 2
	    d 3
	4 entry(s)
	cmd: + e
	e 4
	a 0
	  b 1
	  c 2
	    d 3
	5 entry(s)
	cmd: + f
	e 4
	  f 5
	a 0
	  b 1
	  c 2
	    d 3
	6 entry(s)
	cmd: + g
	g 6
	e 4
	  f 5
	a 0
	  b 1
	  c 2
	    d 3
	7 entry(s)
	cmd: + h
	a 0
	  b 1
	  c 2
	    d 3
	  e 4
	    f 5
	    g 6
	      h 7
	8 entry(s)
	cmd: + i
	i 8
	a 0
	  b 1
	  c 2
	    d 3
	  e 4
	    f 5
	    g 6
	      h 7
	9 entry(s)
	cmd: ?
	minimum: a 0
	i 8
	a 0
	  b 1
	  c 2
	    d 3
	  e 4
	    f 5
	    g 6
	      h 7
	9 entry(s)
	cmd: = 7 c
	i 8
	a 0
	  b 1
	  c 2
	    d 3
	  c 7
	    f 5
	    e 4
	      g 6
	9 entry(s)
	cmd: !
	minimum: a 0
	b 1
	  i 8
	  c 2
	    d 3
	  c 7
	    f 5
	    e 4
	      g 6
	8 entry(s)
	cmd: - 2
	i 8
	b 1
	  d 3
	c 7
	  f 5
	  e 4
	    g 6
	7 entry(s)
	cmd: + j
	b 1
	  d 3
	  i 8
	    j 9
	  c 7
	    f 5
	    e 4
	      g 6
	8 entry(s)
	cmd: = 7 k
	b 1
	  f 5
	  e 4
	    g 6
	  d 3
	    k 7
	    i 8
	      j 9
	8 entry(s)
	cmd: q
	END

echo "Differences between expected (<) and actual (>) output:"
diff -b binheaptest.exp binheaptest.out &&
echo "No differences found"
