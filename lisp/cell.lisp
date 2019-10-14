(begin
	(define Keys (quote (a b c d)))
	(define Values (list 1 2 3 4))

	(print Keys Values)
	(define Env1 (env:capture Keys Values (env:current)))
	(print Env1)
	(env:dump Env1)
	(print (env:get (quote a) Env1))

	(define Env2 (env:capture (quote X) Values Env1))
	(env:dump Env2)
	(print (env:get (quote X) Env2))
)
