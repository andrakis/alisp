;; Advanced pattern matching
;;
;; Erlang-style pattern matching.
;; Underscore '_ is a wildcard and matches anything.
(begin
	;; Convenience macros
	(define defun (fastmacro (Name Arguments Body)
		(list 'define Name (list 'lambda Arguments Body))))

	;; Use Erlang-style list functions to build a dictionary.
	;; A dictionary is just a list, with values in the form:
	;;  (Key Value)
	
	;; Find an item in a list using specified key and key index.
	;; @param Key       The key to find
	;; @param KeyIndex  The location in each item to find the key
	;; @param List      The list of items in the form: ( (Key Value) ... )
	;; @param Comparer  The function to use to compare, or = for standard compare
	;; @return          The full item found (ie, (Key Value)) or nil
	(defun lists:keyfind (Key KeyIndex List Comparer) (begin
		(if (not (env:defined 'Comparer))
			(define Comparer =))
		(if (empty? List)
			'nil
			(next lists:keyfind/5 Key KeyIndex (head List) (tail List) Comparer))
	))

	;; Find an item in a list using specified key and key index.
	;; @param Key       The key to find
	;; @param KeyIndex  The location in each item to find the key
	;; @param List      The list of items in the form: ( (Key Value) ... )
	;; @param CompareKey The function to use to compare keys
	;; @return          The full item found (ie, (Key Value)) or nil
	(defun lists:keyfind/5 (Key KeyIndex Item ListTail CompareKey)
		(if (CompareKey Key (index Item KeyIndex))
			Item
			(if (empty? ListTail)
				'nil
				(next lists:keyfind/5 Key KeyIndex (head ListTail) (tail ListTail)))
		)
	)

	;; Macro that implements logical and.
	;; This logical and is short-circuited, that is if the first condition
	;; is not met, the following conditions, if any, are not evaluated.
	;; (and [cond] [rest...]) ->
	;;   (if [cond] (and [rest...]) false)
	;; Note that this macro takes a variable number of arguments.
	;; Note that this macro does not recursively call itself,
	;; rather it expands during each call exactly once.
	(define and (fastmacro Conds (begin
		(if (empty? Conds)
			'true
			;; (if cond (and rest...) false)
			(list 'if (head Conds)
				(+ (list 'and) (tail Conds))
				'false)))))

	;; Macro that implements logical or.
	;; This logical or is short-circuited, that is if the first condition
	;; is met, the following conditions, if any, are not evaluated.
	;; (or [cond] [rest...]) ->
	;;   (if [cond] true (or [rest...]))
	;; Note that this macro takes a variable number of arguments.
	;; Note that this macro does not recursively call itself,
	;; rather it expands during each call exactly once.
	(define or (fastmacro Conds
		(if (empty? Conds)
			'false
			;; (if cond true (or rest...))
			(list 'if (head Conds)
				'true
				(+ (list 'or) (tail Conds))))))

	;; List Pattern Matching algorithm (LPMa)
	(define pattern#matches (list
		;; Two lists are compared for equality, recursively
		;; calling (pattern...) on each item.
		(list 'listlist (lambda (A B)
			(if (and (empty? A) (empty? B))
				'true
				(if (pattern (head A) (head B))
					(pattern (tail A) (tail B))
					'false))))
		;; Underscore and list always matches.
		(list '_list    (lambda (A B) 'true))
		;; List and underscore always matches.
		(list 'list_    (lambda (A B) 'true))
		;; For any other set, use standard equality operator.
		(list '_        (lambda (A B) (= A B)))
	))
	(defun under_or (Key) (if (= '_ Key) '_ (typeof Key)))
	(defun pattern-key (KeyA KeyB) (+ "" (under_or KeyA) (under_or KeyB)))
	(defun pattern (Key Item) (begin
		(define MatchKey (pattern-key Key Item))
		;;(print "Match key:" MatchKey)
		(define Result (lists:keyfind MatchKey 0 pattern#matches))
		(if (= nil Result)
			(set! Result (lists:keyfind '_ 0 pattern#matches)))
		;;(print "Result:" (index Result 1))
		((index Result 1) Key Item)
	))

	(define Sample '(
		("foo" "bar")
		(1 2)
	))
	(print (lists:keyfind "foo" 0 Sample))
	(print (pattern-key (list) (list)))
	(print (pattern-key '_ (list)))
	(print (pattern-key (list) '_))
	(print (pattern (list) (list)))
	(print (pattern '_ (list)))
)
