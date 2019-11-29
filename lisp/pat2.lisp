;; Advanced pattern matching
;;
;; Erlang-style pattern matching.
;; Underscore '_ is a wildcard and matches anything.
(begin

	;; Use Erlang-style list functions to build a dictionary.
	;; A dictionary is just a list, with values in the form:
	;;  (Key Value)
	
	;; Find an item in a list using specified key and key index.
	;; @param Key       The key to find
	;; @param KeyIndex  The location in each item to find the key
	;; @param List      The list of items in the form: ( (Key Value) ... )
	;; @param Comparer  The function to use to compare, or = for standard compare
	;; @return          The full item found (ie, (Key Value)) or nil
	(define lists:keyfind (lambda (Key KeyIndex List Comparer) (begin
		(if (not (env:defined 'Comparer))
			(define Comparer =))
		(if (empty? List)
			nil
			(next lists:keyfind/5 Key KeyIndex (head List) (tail List) Comparer))
	)))

	;; Find an item in a list using specified key and key index.
	;; @param Key       The key to find
	;; @param KeyIndex  The location in each item to find the key
	;; @param List      The list of items in the form: ( (Key Value) ... )
	;; @param CompareKey The function to use to compare keys
	;; @return          The full item found (ie, (Key Value)) or nil
	(define lists:keyfind/5 (lambda (Key KeyIndex Item ListTail CompareKey)
		(if (CompareKey Key (index Item KeyIndex))
			Item
			(if (empty? ListTail)
				nil
				(next lists:keyfind/5 Key KeyIndex (head ListTail) (tail ListTail)))
		)
	))

	;; Macro that implements logical and.
	;; This logical and is short-circuited, that is if the first condition
	;; is not met, the following conditions, if any, are not evaluated.
	;; (and [cond] [rest...]) ->
	;;   (if [cond] (and [rest...]) false)
	;; Note that this macro takes a variable number of arguments.
	(define and (fastmacro conds (begin
		(if (empty? conds)
			true
			;; (if cond (and rest...) false)
			(list 'if (head conds)
				(+ (list 'and) (tail conds))
				false)))))

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

	(define pattern#matches (list
		(list 'listlist (lambda (A B)
			(if (and (empty? A) (empty? B))
				'true
				(if (pattern (head A) (head B))
					(pattern (tail A) (tail B))
					'false))))
		(list '_list    (lambda (A B) 'true))
		(list 'list_    (lambda (A B) 'true))
		(list '_        (lambda (A B) (= A B)))
	))
	(define under_or (lambda (Key) (if (= '_ Key) '_ (typeof Key))))
	(define pattern-key (lambda (KeyA KeyB) (+ "" (under_or KeyA) (under_or KeyB))))
	(define pattern (lambda (Key Item) (begin
		(define MatchKey (pattern-key Key Item))
		;;(print "Match key:" MatchKey)
		(define Result (lists:keyfind MatchKey 0 pattern#matches))
		(if (= nil Result)
			(set! Result (lists:keyfind '_ 0 pattern#matches)))
		;;(print "Result:" (index Result 1))
		((index Result 1) Key Item)
	)))

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
