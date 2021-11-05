;;;
;;; hostile_2_2.lisp
;;;


;; A large ship designed mostly for boarding. It only carries one real weapon, a
;; single missile silo. But with six crew and three transporters, this level can
;; give unprepared players some serious trouble.


(init-opponent 8 'hostile)



(configure-player
 (opponent)
 '((power-core 4 13)
   (power-core 4 11)
   (stairwell 6 11)
   (transporter 7 13)
   (hull 7 12)
   (transporter 3 11)
   (transporter 2 11)
   (infirmary 2 13)
   (hull 0 14)
   (hull 0 13)
   (hull 0 12)
   (hull 0 11)
   (hull 1 14)
   (hull 1 13)
   (missile-silo 1 11)
   (ion-cannon 2 10)
   (hull 3 10)
   (hull 5 10)
   (hull 4 10)
   (hull 6 10)
   (hull 2 9)
   (hull 3 9)))


(show-flag (opponent))


(map
 (lambda
   (add-chr (opponent) (car $0) (cdr $0) 'hostile 0))
 '((4 . 14)
   (5 . 14)
   (4 . 12)
   (5 . 12)
   (6 . 14)
   (6 . 13)))
