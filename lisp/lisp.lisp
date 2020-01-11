;; Lisp-like functions
(begin
	(define defun (fastmacro (Name Args Body)
		(list 'define Name (list 'lambda Args Body))))
	(define car (lambda (L) (head L)))
	(define cdr (lambda (L) (tail L)))
)
