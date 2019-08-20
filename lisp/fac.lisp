(begin
	(define fac (lambda (n) (fac/2 n 1)))
	(define fac/2 (lambda (n a)
		(if (= 1 n) a (fac/2 (- n 1) (* n a)))))
	(define x 10)
	(print "Fac" x (fac x))
	;;(debug:repl)
)
