;; Test of macros
(begin
	;; Macro that rewrites (?def symbol) to (env:defined (quote symbol)).
	;; Makes it so one does not need to (quote) their symbol name.
	(define def? (macro (sym) (list (quote env:defined) 
		(list (quote quote) sym))))

	;; Macro that implements a while loop.
	;; (while [cond] [body])
	;; Rewrites your code to:
	;;   (if cond (begin body (do cond body)))
	(define while (macro (cond body)
		(list (quote if) cond (list (quote begin)
			body
			(list (quote while) cond body)))))

	;; Macro that implements logical or.
	;; This logical or is short-circuited, that is if the first condition
	;; is met, the following conditions, if any, are not evaluated.
	;; (or [cond] [rest...]) ->
	;;   (if [cond] true (or [rest...]))
	;; Note that this macro takes a variable number of arguments.
	(define or (macro conds (begin
		(if (empty? conds)
			false
			(begin ;; else
				(print "Testing condition" (head conds))
				;; (if cond true (or rest...))
				(list (quote if) (head conds)
					true
					(+ (list (quote or)) (tail conds))))))))

	;; Macro that implements logical and.
	;; This logical and is short-circuited, that is if the first condition
	;; is not met, the following conditions, if any, are not evaluated.
	;; (and [cond] [rest...]) ->
	;;   (if [cond] (and [rest...]) false)
	;; Note that this macro takes a variable number of arguments.
	(define and (macro conds (begin
		(if (empty? conds)
			true
			(begin ;; else
				(print "Testing condition" (head conds))
				;; (if cond (and rest...) false)
				(list (quote if) (head conds)
					(+ (list (quote and)) (tail conds))
					false))))))

	;; Increment a variable.
	(define inc! (macro (name value) (begin
		;; if value not supplied, default to 1
		(if (not (def? value)) (define value 1))
		;; Return list that modifies variable
		(list (quote set!) name (list (quote +) name value)))))

	(define a 1)
  	(while (<= a 10) (begin
  		(print a)
  		(inc! a)
  	))

	(print "or" (or
		;; These will run
		(= a 3) (= a 2) (= a 11)
		;; These will not
		(= a 5) (= a 4)
	))

	(define b 2)
	(print "and" (and
		;; All will run
		(= a 11) (= b 2)
	))
)
