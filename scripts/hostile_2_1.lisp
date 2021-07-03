;;;
;;; hostile_2_1.lisp
;;;


(init-opponent 7 'hostile)


(configure-player
 (opponent)
 '((power-core 2 13)
   (power-core 4 13)
   (cannon 1 14)
   (cannon 1 13)
   (cannon 1 12)
   (forcefield 0 14)
   (forcefield 0 13)
   (forcefield 0 12)
   (hull 1 11)
   (hull 1 10)
   (hull 2 12)
   (hull 2 11)
   (ion-cannon 2 10)
   (ion-cannon 2 9)
   (hull 2 8)
   (infirmary 3 11)
   (transporter 5 11)
   (hull 5 10)
   (hull 5 9)
   (hull 6 10)
   (stairwell 6 11)
   (power-core 3 9)
   (hull 3 8)
   (hull 4 8)))


(show-flag (opponent))



(add-hostile-chr (opponent) 3 14)
(add-hostile-chr (opponent) 2 14)
(add-hostile-chr (opponent) 3 12)
