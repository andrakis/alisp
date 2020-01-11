(begin
	(define map (lambda (List Observer)
		(map/3 List Observer (list))))
	(define map/3 (lambda (List Observer Acc)
		(if (empty? List)
			Acc
			(begin
				(map/3 (tail List) Observer (+ Acc (Observer (head List))))
			)
		)
	))
	(define foldl (lambda (List Acc Observer)
		(if (empty? List)
			Acc
			(foldl (tail List) (+ Acc (Observer (head List))))
		)
	))

	(print (map (list 1 2 3 4) (lambda (x) (+ 1 x))))

	(define part2 (lambda () (begin
		(define switch (lambda args (begin
			(define value (head args))
			(define cases (tail args))
			(switch:case value (head cases) (tail cases))
		)))
		(define case (macro (cond body) (list cond body)))

		(define observer (lambda (x)
			(switch x
				(case 0 "Zero")
				(case 1 "One")
				(case 2 "Two")
				(case 3 "Three")
				(case 4 "Four")
				(case 5 "Five")
				(case 6 "Six")
				(case 7 "Seven")
				(case 8 "Eight")
				(case 9 "Nine")
				(case true " ")
			)))
		(define sequ (list 0 4 5 1))
		(print (map sequ (lambda (x) (observer x))))
	)))
	(part2)
)
