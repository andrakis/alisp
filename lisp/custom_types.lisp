(begin
	(define oldTypeof typeof)
	(define CustomType (quote `custom`type))
	(define custom`type (lambda (type v) (list CustomType (list type v))))

	(define typeof (lambda (x)
		(if (= (quote list) (oldTypeof x))
			(if (= CustomType (head x))
				(head (head (tail x)))
				(oldTypeof x))
			(oldTypeof x))
	))

	(define X (custom`type (quote tuple) (list 1 2 3)))
	(print (typeof X))
	(print (typeof 123))
)
