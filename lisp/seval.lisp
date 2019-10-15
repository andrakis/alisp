;; Simplest eval for debugging.
;; Provides a very basic evaluator that allows for better debugging.
;; Usage:
;;   [alisp] seval.lisp [file to run.lisp]
;;
(begin
	;; Repeat String given Count times
	(define repeat (lambda (String Count) (repeat/3 String Count "")))
	(define repeat/3 (lambda (String Count Acc)
		(if (> Count 0)
			(repeat/3 String (- Count 1) (+ Acc String))
			Acc
		)
	))

	(define map (lambda (List Observer)
		(map/3 List Observer (list))))
	(define map/3 (lambda (List Observer Acc)
		(if (empty? List)
			Acc
			(begin
				(map/3 (tail List) Observer (+ Acc (list (Observer (head List)))))
			)
		)
	))

	;; Macro that rewrites (?def symbol) to (env:defined (quote symbol)).
	;; Makes it so one does not need to (quote) their symbol name.
	(define def? (macro (Sym) (list (quote env:defined) 
		(list (quote quote) Sym))))

	;; Macro that implements logical or.
	;; This logical or is short-circuited, that is if the first condition
	;; is met, the following conditions, if any, are not evaluated.
	;; (or [cond] [rest...]) ->
	;;   (if [cond] true (or [rest...]))
	;; Note that this macro takes a variable number of arguments.
	(define or (macro Conds
		(if (empty? Conds)
			false
			;; (if cond true (or rest...))
			(list (quote if) (head Conds)
				true
				(+ (list (quote or)) (tail Conds))))))

	(define Depth 0)
	;; controlled by -s flag
	(define Silent false) ;; whether to print (dbg) messages
	(define seval (lambda (X Env) (begin
		(if (not (def? Env))
			(define Env (env:new)))
		(set! Depth (+ 1 Depth))
		;;(dbg "(seval" (str-limit X) Env ")")
		(define Result (do-next X Env))
		(dbg "(seval" (str-limit X) Env ") =>" (str-limit Result))
		(set! Depth (- Depth 1))
		Result
	)))
	(define str-limit (lambda (Str) (begin
		(set! Str (+ "" Str))
		(if (> (length Str) 40)
			(+ (string:substr Str 0 37) "...")
			Str))))
	(define dbg (lambda Args
		(if (not Silent)
			(print (depthness) (repeat "-" Depth) Args))))
	(define depthness (lambda ()
		(if (<= Depth 9) Depth "+")))

	(define do-next (lambda (X Env) (begin
		(dbg "(do-next" (str-limit X) Env)
		(if (or (= nil X) (simple? X))
			X
			(if (= (quote atom) (typeof X))
				(env:get X Env)
				(if (empty? X)
					nil
					(do-complex X Env))))
	)))
	(define simple? (lambda (X) (begin
		(define T (typeof X))
		(or (= T (quote number)) (= T (quote string)))
	)))
	(define do-complex (lambda (X Env) (begin
		;; (dbg "(do-complex" X Env ")")
		(if (= (quote atom) (typeof (head X)))
			(do-builtin X Env)
			(do-proc (head X) (tail X) Env))
	)))
	(define do-builtin (lambda (X Env) (begin
		;; (dbg "(do-builtin" X Env ")")
		(define Xh (head X))
		(if (= (quote quote) Xh)
			(index X 1)
			(if (= (quote if) Xh)
				(do-if (tail X) Env)
				(if (= (quote define) Xh)
					(env:define (index X 1) (seval (index X 2) Env) Env)
					(if (= (quote set!) Xh)
						(env:set! (index X 1) (seval (index X 2) Env) Env)
						(if (= (quote lambda) Xh)
							(cell:lambda (index X 1) (index X 2) Env)
							(if (= (quote macro) Xh)
								(cell:macro (index X 1) (index X 2) Env)
								(if (= (quote begin) Xh)
									(do-begin (tail X) Env)
									;; else
									(do-proc (head X) (tail X) Env)
								)
							)
						)
					)
				)
			)
		)
	)))
	(define do-if (lambda (Args Env) (begin
		;; (dbg "do-if" Args Env)
		(define Test (index Args 0))
		(define Conseq (index Args 1))
		(define Alt (index Args 2))
		(define Result (seval Test Env))
		(dbg "if" (str-limit Test) "=>" Result)
		(if TailRecursive
			(do-next (if (= false Result) Alt Conseq) Env)
			(seval (if (= false Result) Alt Conseq) Env))
	)))
	(define do-begin (lambda (Bodies Env) (begin
		;; (dbg "do-begin" Bodies)
		(if (empty? (tail Bodies))
			(do-next (head Bodies) Env)
			(begin
				(seval (head Bodies) Env)
				(do-begin (tail Bodies) Env)))
	)))
	(define do-proc (lambda (Proc0 Exps0 Env0) (begin
		;; (dbg "(do-proc" Proc0 Exps0 Env0 ")")
		(define Proc1 (seval Proc0 Env0))
		;; (dbg "(do-proc" Proc1 ")")
		(define Pt (typeof Proc1))
		(define Exps
			(if (= (quote macro) Pt)
				Exps0
				(map Exps0 (lambda (E) (seval E Env0)))))
		;; (dbg "exps" Exps)
		(if (= (quote lambda) Pt) (begin
			;; (dbg "is lambda")
			(define Env1 (env:capture
				(cell:lambda_args Proc1)
				Exps
				(cell:lambda_env  Proc1)))
			(if TailRecursive
				(do-next (cell:lambda_body Proc1) Env1)
				(seval (cell:lambda_body Proc1) Env1))
		) (if (= (quote macro) Pt) (begin
			;; (dbg "is macro")
			(define Env1 (env:capture
				(cell:lambda_args Proc1)
				Exps
				(cell:lambda_env  Proc1)))
			(define MR (seval (cell:lambda_body Proc1) Env1))
			(dbg "macro result:" MR)
			(if TailRecursive
				(do-next MR Env0)
				(seval MR Env0))
		) (if (= (quote proc) Pt) (begin
			;; (dbg "is proc with exps" Exps)
			(cell:proc Proc1 Exps)
		) (if (= (quote proc_env) Pt) (begin
			;; (dbg "is procenv")
			(cell:proc_env Proc1 Exps Env0)
		) (begin
			(error "Not an executable cell")
		)))))
	)))

	;; tail recursive factorial
	(define fac (lambda (n) (fac/2 n 1)))
	(define fac/2 (lambda (n a)
		(if (= 1 n) a (fac/2 (- n 1) (* n a)))))

	(define Code (quote (fac 10)))
	;; (define Code (quote (seval fac 10)))

	(define File "")
	(define Args (list))
	(define ArgsOnly false) ;; pass -- and all following items are passed as arguments
	(define TailRecursive false) ;; pass -t to enable, makes readout difficult to parse
	(define parse-args (lambda (args) (begin
		(if (not (empty? args))
			(if (or ArgsOnly (not (= "" File)))
				(set! Args args)
				(begin
					(define arg`h (head args))
					(if (= "-s" arg`h)
						(set! Silent true)
						(if (= "-t" arg`h)
							(set! TailRecursive true)
							(if (= "" File)
								(set! File arg`h) 
								;; all other arguments
								(set! Args (+ Args (list arg`h)))
							)
						)
					)
					(parse-args (tail args))
				)
			)
		))))

	(if (def? argv) (begin
		(if (not (empty? argv)) (begin
			(parse-args argv)
			;; attempt to load file
			(set! Code
				(debug:parse (file:read (file:path File))))
			(print "Loaded from" (file:path File))
			;; update argv
			(set! argv Args)
		))
	))
	(define Result (seval Code (env:new)))
	(print "Result:" Result)
	;;(print "Argument state:")
	;;(print " File:" File "Silent:" Silent "TailRecursive:" TailRecursive)
	;;(print " Arguments passing to child" argv)
)
