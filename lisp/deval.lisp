;; deval.lisp - debug eval
;;
(begin
	(require "macros")

	(define deval<depth 0)  ;; counter of depth
	(define deval<msg (lambda args (
		print (string:repeat deval<depth "-") args)))

	(define deval (lambda (x env) (begin
		(if (deval:simple? x) x
			(if (= (atom atom) (typeof x)) (env:get x env) (deval:complex x env)))
	)))
	(define deval:complex (lambda (x env) (begin
		(define sym (head x))
		(if (= (atom atom) (typeof sym))
			(deval:builtin sym x env)
			(deval:proc x env))
	)))
	(define deval:simple? (lambda (x) (begin
		(define x-t (typeof x))
		(or (= x-t (atom string)) (= x-t (atom number)))
	)))


	(define deval:builtin (lambda (sym x env) (begin
		(switch sym
			(case (atom quote) sym)
			(case (atom if) (deval:if (tail x) env))
			(case (atom define) (deval:define (tail x) env))
			(case (atom set!) (deval:set! (tail x) env))
			(case (atom begin) (deval:begin (tail x) env))
			(case true (deval:proc x env))
		)
	)))

	(deval (list (atom print) (list (atom +) 1 5)) (env:this))
)
