;; Pattern matching for functions (version 1)
(begin
	;; Pattern matching using just atoms right now
	(define fac/pt (env:new))
	(env:define '0 (lambda (N) 1) fac/pt)
	(env:define '_ (lambda (N) (* N (fac (- N 1)))) fac/pt)

	(define label (lambda (Arg)
		(if (env:defined Arg fac/pt)
			(+ "" Arg) ;; Convert to string
			'_
		)))

	(define fac (lambda (N)
		(next (env:get (label N) fac/pt) N)))

	(define X 10)
	(print "Factorial of" X "is" (fac X))
)
