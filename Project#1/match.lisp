(defun match(p d)	
	(setf ans '())
	(rpm p d nil)
	(cond 
			(ans ans) 
			(t t))
)

(defun boundp_(x a)
	(assoc x a)
)

(defun startwith?(x)
	(equal (elt (symbol-name x) 0) #\?)
)
(defun is-vbl(x)
	(cond
		((numberp x) nil)
		(t (startwith? x))
	)
)
(defun rpm(p d a)
	(cond
		((and (null p) (null d)) ;base case 1: exhausted both successfully
			(setf ans (cons a ans)))
		((eql '* (car p)) ;encounted a kleene *
			(or (rpm (cdr p) d a) (rpm p (cdr d) a)))
		((eql '? (car p)) ;encounted a "?"
			(rpm (cdr p) (cdr d) a))
		((or (null p) (null d)) nil) ;base case 2
		((is-vbl (car p)) ;encounted a pattern variable
			(cond
				((boundp_ (car p) a) ;if car p is a bound variable
					(cond
						((equal (assoc (car p) a) (list (car p) (car d))) ;;its bound value is equal to the first data element
							(rpm (cdr p) (cdr d) a))
						(t nil) ; unequal, fail
					))
				(t (rpm (cdr p) (cdr d) (cons (list (car p) (car d)) a))) ;it's unbound, bind it on the association list 
			))	
		((atom (car p))
			(cond 
				((eql (car p) (car d)) (rpm (cdr p) (cdr d) a))
				(t nil)
			))
		(t
			(let ((newa (rpm (car p) (car d) a))) ;pattern match the sublists
				(cond 
					((null newa) nil) ;fail
					((listp newa) (rpm (cdr p) (cdr d) newa)) ;newa is a list
					(t (rpm (cdr p)(cdr d) a)) ; newa is a list of nil
					))
		)
	)
)