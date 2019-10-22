;; recursive switch macro
;; Implements a switch/case statement processor.
;; Improves upon previous switch macro:
;;  - No longer maps all conditions (previously did so - twice)
;;  - Now short-circuited, first successful match prevents further condition
;;    testing.
;;  - Quicker and recursive like other platform macros.
(begin
	(define switch (macro Conds (begin
		(define X (head Conds))
		(define Cond (head (tail Conds)))
		(define CondCheck (index Cond 0))
		(define CondEq (index Cond 1))
		(if (= (index CondCheck 0) (quote case))
			(list (quote if) (list (quote =) X (index CondCheck 1))
				CondEq
				(+ (list (quote switch) X) (tail (tail Conds))))
			(if (= CondCheck true)
				CondEq
				(begin
					(print "Unexpected CaseOrTrue:" CondCheck)
					(error "unexpected"))
			)
		)
	)))

	(define X 5)
	(if (env:defined (quote argv))
		(if (not (empty? argv)) (begin
			(set! X (head argv))
			(print "Using" X "for value"))))

	(switch X
		((case 1) "one")
		((case 2) "two")
		((case 3) "three")
		((case 4) "four")
		(true "not sure (try passing numbers 1 - 4 on command line)"))
)
