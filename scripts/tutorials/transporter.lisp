;;;
;;; transporter.lisp
;;;


(terrain (player) 6)
(configure-player
 (player)
 '((power-core 1 13)
   (transporter 3 13)
   (transporter 4 13)))


(add-chr (player) 1 14 'neutral 0)
(add-chr (player) 2 14 'neutral 0)



(add-coins 20000)


(init-opponent 4 'hostile)

(configure-player
 (opponent)
 '((power-core 1 13)
   (stairwell 3 11)))

(add-chr (opponent) 1 14 'hostile 0)


(autopilot
 '((650 Select)
   (950 Right)
   (683 Right)
   (416 A)
   (317 Right)
   (199 Right)
   (283 A)
   (366 Left)
   (450 Left)
   (199 Left)
   (417 A)
   (433 Right)
   (266 Right)
   (367 A)
   (1616 Left)
   (7116 R)
   (633 Right)
   (333 Right)
   (416 Right)
   (1849 B)
   (383 Left)
   (182 Up)
   (750 A)
   (533 Left)
   (466 A)
   (417 Right)
   (348 Right)
   (183 Right)
   (133 Right)
   (182 Right)
   (200 Right)
   (216 Right)
   (466 Up)
   (533 A)
   (249 Down)
   (216 Down)
   (300 Left)
   (82 Left)
   (283 Left)
   (483 Right)
   (450 A)
   (450 Right)
   (366 Right)
   (1300 Down)
   (283 Left)
   (5917 A)
   (149 Right)
   (199 Right)
   (333 A)
   (299 Left)
   (300 Right)
   (282 Right)
   (133 Down)
   (366 Right)
   (266 A)
   (133 Left)
   (233 A)
   (233 Right)
   (583 Down)
   (133 Right)
   (749 Right)
   (699 A)
   (417 Left)
   (316 A)
   (200 Left)
   (183 Left)
   (182 Left)
   (866 A)
   (516 Right)
   (200 Right)
   (332 Down)
   (416 A)
   (833 Down)
   (449 Left)
   (166 Up)
   (167 Up)
   (283 A)
   (200 Down)
   (366 A)
   (766 Left)
   (583 A)))
