(begin
	(define or (fastmacro Conds
		(if (empty? Conds)
			false
			;; (if cond true (or rest...))
			(list (quote if) (head Conds)
				true
				(+ (list (quote or)) (tail Conds))))))

	(print (or (= 1 2) (= 1 3))))

