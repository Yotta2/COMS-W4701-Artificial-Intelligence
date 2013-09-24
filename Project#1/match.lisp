(defun match(p d)	
	(rpm p d ())
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
		((and p (listp (car p)))	     ; (car p) is a list
			(cond
				((listp (car d)) ;(car d) is a list
					(let ((a-lists (rpm (car p) (car d) a)))
						(cond
							((atom a-lists) 
								(cond 
									((equal a-lists t) (rpm (cdr p) (cdr d) ()))
									(t nil)
								)
							)
							((null a-lists) (rpm (cdr p) (cdr d) ()))
							(t (enum (cdr p) (cdr d) a-lists))
						)
					)
				)
				(t nil) 
			)
		)
		((and (null p) (null d)) ;base case 1: exhausted both successfully
			(cond
				(a (cons a ()))
				(t t)
			)
		)
		((null d)
			(cond
				((equal (car p) '*) (rpm (cdr p) d a))
				(t nil)
			)
		)
		((eql '* (car p)) ;encountered a kleene *
			(let ((ans1 (rpm (cdr p) d a)) (ans2 (rpm p (cdr d) a)))
				(cond
					((and (atom ans1) (atom ans2)) (or ans1 ans2))
					((and (listp ans1) (listp ans2)) (append ans1 ans2))
					((listp ans1) (ans1))
					((listp ans2) (ans2))
				)
			 )
		)
		((eql '? (car p)) ;encountered a "?"
			(rpm (cdr p) (cdr d) a)
		)
		((or (null p) (null d)) nil) ;base case 2: only one of them is nil
		((is-vbl (car p)) ;encountered a pattern variable
			(cond
				((boundp_ (car p) a) ;if car p is a bound variable
					(cond
						((equal (assoc (car p) a) (list (car p) (car d))) ;;its bound value is equal to the first data element
							(rpm (cdr p) (cdr d) a))
						(t nil) ; unequal, fail
					))
				(t (rpm (cdr p) (cdr d) (cons (list (car p) (car d)) a))) ;it's unbound, bind it on the association list 
			)
		)	
		((atom (car p))  ; encountered an atom/symbol
			(cond 
				((equal (car p) (car d)) (rpm (cdr p) (cdr d) a))
				(t nil)
			)
		)
	)
)

(defun enum(p d a-lists) ;enumerate all scenarios(a) in a-lists; a-lists should not the null for the first call
	(cond
		((null a-lists) nil)
		(t (let ((hd (rpm p d (car a-lists))) (rst (enum p d (cdr a-lists))))
				(cond
					((and (atom hd) (atom rst)) (or hd rst))
					((and (listp hd) (listp rst)) (append hd rst))
					((listp hd) (hd))
					((listp rst) (rst))
				)
			)
		)
	)
)