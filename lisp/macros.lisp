;; Test of macros
(begin
	;; Macro that rewrites (?def symbol) to (env:defined (quote symbol)).
	;; Makes it so one does not need to (quote) their symbol name.
	(define def? (macro (sym) (list (quote env:defined) 
		(list (quote quote) sym))))

	;; Macro that implements a do loop.
	;; Rewrites your code to:
	;;   (if cond (begin body (do cond body)))
	(define do (macro (cond body)
		(list (quote if) cond (list (quote begin)
			body
			(list (quote do) cond body)))))

	;; Increment a variable.
	(define inc! (macro (name value) (begin
		;; if value not supplied, default to 1
		(if (not (def? value)) (define value 1))
		;; Return list that modifies variable
		(list (quote set!) name (list (quote +) name value)))))

	(define a 1)
	(do (<= a 10) (begin
		(print a)
		(inc! a)
	))

)
