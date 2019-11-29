;; Provides a feature complete evaluator that allows for better debugging.
;; Can evaluate itself, very slowly. When evaluating itself, -s and -t are
;; often used.
;;
;; Usages:
;;   [alisp] teval.lisp [-s] [file to run.lisp]
;;   [alisp] teval.lisp teval.lisp -s [file to run.lisp]
;;
;; Where:
;;   -s                 Silences debug message output
;;
;;
(begin
	;; Map a List, calling Observer on each item and returning a new list
	;; with the results of each Observer call.
	(define map (lambda (List Observer) (next map/3 List Observer (list))))
	;; Tail recursive map implementation.
	(define map/3 (lambda (List Observer Acc)
		(if (empty? List)
			Acc
			(next map/3 (tail List) Observer (+ Acc (list (Observer (head List)))))
		)
	))

	;; Macro that rewrites (?def symbol) to (env:defined (quote symbol)).
	;; Makes it so one does not need to (quote) their symbol name.
	(define def? (fastmacro (Sym) (list 'env:defined (list 'quote Sym))))

	;; Macro that implements logical or.
	;; This logical or is short-circuited, that is if the first condition
	;; is met, the following conditions, if any, are not evaluated.
	;; (or [cond] [rest...]) ->
	;;   (if [cond] true (or [rest...]))
	;; Note that this macro takes a variable number of arguments.
	(define or (fastmacro Conds
		(if (empty? Conds)
			false
			;; (if cond true (or rest...))
			(list 'if (head Conds)
				true
				(+ (list 'or) (tail Conds))))))

	;; Evaluation depth. Recursive calls to seval increase this.
	(define Depth 0)
	;; inverted flags so (not /flag/) is not needed
	;; controlled by -s flag
	(define NotSilent true) ;; whether to print (dbg) messages

	;; Main evaluation function
	(define seval (lambda (X Env) (begin
		(set! Depth (+ 1 Depth))
		(if NotSilent
				(dbg "(seval  " (str-limit X) Env ")"))
		(define Result (do-next X Env))
		(dbg "(seval  " (str-limit X) Env ") =>" (str-limit Result))
		(set! Depth (- Depth 1))
		Result
	)))

	;; limits expression output to given number of characters
	(define DbgStringLimit 40)
	;; used by str-limit when grabbing substring to place the "..."
	(define DbgStringLimitAdj (- DbgStringLimit 3))

	;; Convert arguments to string, and limit result to DbgStringLimit characters
	;; -spec str-limit(any()) -> str(DbgStringLimit)
	(define str-limit (lambda (Str) (begin
		(set! Str (+ "" Str)) ;; convert args to string, even if they arent
		(if (> (length Str) DbgStringLimit)
			(+ (string:substr Str 0 DbgStringLimitAdj) "...")
			Str))))

	;; Print a debug message, along with the depth counter.
	;; -spec dbg(any()) -> nil()
	(define dbg (lambda Args
		(if NotSilent
			(print (depthness) (string:repeat "-" Depth) Args))))

	;; Returns the depth number of the current evaluation. If number
	;; is more than 10, returns just a "+" to ensure indentation remains correct.
	;; -spec depthness() -> num() | "+"
	(define depthness (lambda ()
		(if (<= Depth 9) Depth "+")))

	(define TypeSelector (env:new))
	(env:define 'number (lambda (X Env) X) TypeSelector)
	(env:define 'string (lambda (X Env) X) TypeSelector)
	(env:define 'atom   (lambda (X Env) (env:get X Env)) TypeSelector)

	;; Perform the next step in evaluation. Essentially the same as calling
	;; seval, but does not increase/decrease depth counter.
	;; -spec do-next(expr(), env()) -> any()
	(define do-next (lambda (X Env) (begin
		(if NotSilent
			(dbg "(do-next" (str-limit X) Env))
		(define T (typeof X))
		(if (env:defined? T TypeSelector)
			(next (env:get T TypeSelector) X Env)
			(if (empty? X)
				nil
				(next do-complex X Env)))
	)))

	;; Perform the complex evaluation of the given expression.
	;; The expression is either a builtin or a procedure call.
	(define do-complex (lambda (X Env)
		(if (= 'atom (typeof (head X)))
			(next do-builtin X Env)
			(next do-proc (head X) (tail X) Env false))
	))

	(define Builtins (env:new))
	(env:define 'next      (lambda (X Env) (next do-proc-next (tail X) Env)) Builtins)
	(env:define 'if        (lambda (X Env) (next do-if (tail X) Env)) Builtins)
	(env:define 'begin     (lambda (X Env) (next do-begin (tail X) Env)) Builtins)
	(env:define 'define    (lambda (X Env) (env:define (index X 1) (seval (index X 2) Env) Env)) Builtins)
	(env:define 'set!      (lambda (X Env) (env:set! (index X 1) (seval (index X 2) Env) Env)) Builtins)
	(env:define 'lambda    (lambda (X Env) (cell:lambda (index X 1) (index X 2) Env)) Builtins)
	(env:define 'macro     (lambda (X Env) (cell:macro (index X 1) (index X 2) Env)) Builtins)
	(env:define 'fastmacro (lambda (X Env) (cell:fastmacro (index X 1) (index X 2) Env)) Builtins)

	;; If the first item in X is identified as an atom, check if it matches
	;; a builtin. If not, do-proc is used an the expression treated as a function
	;; call.
	(define do-builtin (lambda (X Env)
		(next do-builtin-check X (head X) Env)))
	(define do-builtin-check (lambda (X Xh Env)
		(if (env:defined Xh Builtins)
			(next (env:get Xh Builtins) X Env)
			;; else
			(next do-proc (head X) (tail X) Env false)
		)
	))

	;; Perform the builtin "(if Test Conseq Alt)".
	;; The result of the if statement is then evaluated, and tail recursion
	;; can occur here.
	(define do-if (lambda (Args Env) (begin
		;; (dbg "do-if" Args Env)
		(define Test (index Args 0))
		(define Conseq (index Args 1))
		(define Alt (index Args 2))
		(define Result (seval Test Env))
		(define Return (if (= false Result) Alt Conseq))
		(if NotSilent
			(dbg "if" (str-limit Test) (str-limit Conseq) (str-limit Alt) "=>" (str-limit Return)))
		(next do-next Return Env)
	)))

	;; Perform the builtin "(begin ...)".
	;; The final item in the begin block is used for tail recursion, and
	;; is used as the return value for the begin block.
	(define do-begin (lambda (Bodies Env)
		(if (empty? (tail Bodies))
			(next do-next (head Bodies) Env)
			;; Hack: call do-begin with too many arguments, to avoid a begin
			;; block. Element in Bodies will still be evaluated, just not stored
			;; anywhere.
			(next do-begin (tail Bodies) Env (seval (head Bodies) Env)))
	))

	;; Call a proc in the "next" mode, which means to reuse the current
	;; environment for the next function call, instead of creating a new
	;; environment.
	(define do-proc-next (lambda (X Env)
		(next do-proc (head X) (tail X) Env true)))

	(define ProcExps (env:new))
	(env:define 'macro (lambda (Exps Env) Exps) ProcExps)
	(env:define 'fastmacro (lambda (Exps Env) Exps) ProcExps)
	(env:define '_ (lambda (Exps Env) (map Exps0 (lambda (E) (seval E Env0)))) ProcExps)

	;; Call a proc, be it lambda, macro, proc, or procenv.
	;; Unless a macro, all arguments are first evaluated.
	;; If a lambda or macro, a new environment is created to capture the arguments.
	;; If a lambda or macro, tail recursion can be used here.
	(define do-proc (lambda (Proc0 Exps0 Env0 IsNext) (begin
		;; (dbg "(do-proc" Proc0 Exps0 Env0 ")")
		(define Proc1 (seval Proc0 Env0))
		;; (dbg "(do-proc" Proc1 ")")
		(define Pt (typeof Proc1))
		(define Exps
			((env:get (if (env:defined? Pt ProcExps) Pt '_) ProcExps) Exps0 Env))
		;; (dbg "exps" Exps)
		(if (= 'lambda Pt) (begin
			;; (dbg "is lambda")
			(define Env1
				(if IsNext
					(env:recapture Env0 (cell:lambda_args Proc1) Exps)
					(env:capture (cell:lambda_args Proc1) Exps (cell:lambda_env Proc1))))
			(next do-next (cell:lambda_body Proc1) Env1)
		) (if (= 'macro Pt) (begin
			;; (dbg "is macro")
			(define Env1
				(if IsNext
					(env:recapture Env0 (cell:lambda_args Proc1) Exps)
					(env:capture (cell:lambda_args Proc1) Exps (cell:lambda_env Proc1))))
			(define MR (seval (cell:lambda_body Proc1) Env1))
			(dbg "macro result:" MR)
			(next do-next MR Env0)
		) (if (= 'fastmacro Pt) (begin
			(dbg "is fast macro")
			(env:recapture (cell:lambda_env Proc1) (cell:lambda_args Proc1) Exps)
			(define MR (seval (cell:lambda_body Proc1) (cell:lambda_env Proc1)))
			(dbg "fastmacro result:" MR)
			(next do-next MR Env0)
		) (if (= 'proc Pt)
			(cell:proc Proc1 Exps)
		(if (= 'proc_env Pt)
			(cell:proc_env Proc1 Exps Env0)
			(error "Not an executable cell")
		)))))
	)))

	;; tail recursive factorial
	(define fac (lambda (n) (next fac/2 n 1)))
	(define fac/2 (lambda (n a)
		(if (= 1 n) a (next fac/2 (- n 1) (* n a)))))

	;; Local variables
	(define Code '(fac 10))
	(define File "")
	(define Args (list))
	(define ArgsOnly false) ;; pass -- and all following items are passed as arguments

	;; Parse command-line arguments and update variables.
	(define parse-args (lambda (args) (begin
		(if (not (empty? args))
			(if (or ArgsOnly (not (= "" File)))
				(set! Args args)
				(begin
					(define arg`h (head args))
					(if (= "-s" arg`h)
						(set! NotSilent false)
						(if (= "" File)
							(set! File arg`h) 
							;; all other arguments
							(set! Args (+ Args (list arg`h)))
						)
					)
					(next parse-args (tail args))
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
	;;(print " File:" File "NotSilent:" NotSilent "TailRecursive:" TailRecursive)
	;;(print " Arguments passing to child" argv)
)


