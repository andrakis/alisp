(begin

	(define defun (fastmacro (Name Args Body)
		(list 'define Name (list 'lambda Args Body))))

	(defun show-help () (begin
		(print "Usage:")
		(print "[alisp] listavg.lisp [item...]")
		(print "Where:")
		(print "	item		A number")
		(print "	item...		A set of numbers")
	))

	(defun process-args (Args)
		(if (empty? Args)
			nil
			(next process-args-arg (head Args) (tail Args))))
	(defun process-args-arg (Arg Args)
		(if (= 'number (typeof Arg))
			(begin
				(set! DoAnything true)
				(set! List (+ List (list Arg)))
				(next process-args Args)
			)
			(next process-error-arg Arg Args)))
	(defun process-error-arg (Arg Args) (begin
		(print "Ignoring unknown option:" Arg)
		(next process-args Args)))

	(define main (lambda () (begin
		(print "List is:" List)
		(define Total 0)
		(define Count 0)
		(define Observer (lambda (Item) (begin
			(set! Total (+ Total Item))
			(set! Count (+ Count 1)))))
		(define each (lambda (List Observe)
			(if (empty? List) nil
				(next each (tail List) Observe (Observe (head List))))))
		(each List Observer)
		(print "Total:" Total)
		(print "Count:" Count)
		(print "Average:" (/ Total Count))
	)))

	(define DoAnything false)
	(define ShowHelp false)
	(define List (list))
	(if (env:defined 'argv) (process-args argv))
	(if (not DoAnything) (set! ShowHelp true))

	(if ShowHelp
		(show-help)
		(main)
	)
)
