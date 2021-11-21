;;;
;;; resurrect.lisp
;;;
;;; Resurrect a snapshot of one of the player's previously-destroyed castles.
;;;


;; Look into the array of snapshots per zone
(def temp (get snapshots (zone)))


;; Pick a random snapshot
(def temp (get temp (choice (length temp))))


;; Find the highest x-value in the room data (We want to mirror the room over
;; the y-axis).
(def max-x 0)
(map (lambda
       (if (> (get $0 1) max-x)
           (def max-x (get $0 1))))
     temp)


;; Flip the room (the enemy must face the player)
(def temp
     (map (lambda
            (list (get $0 0)
                  (- highest (get $0 1))
                  (get $0 2)))
          temp))
