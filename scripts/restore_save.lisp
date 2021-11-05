;;;
;;; The game invokes this script when loading a save file. The script should
;;; contain a single s-expression, a lambda, which will receive data written by
;;; save.lisp.
;;;


(lambda
  (eval-other-file "reset_hooks.lisp")

  (terrain (player) (get $0 5))

  (configure-player
   (player)
   (get $0 0)) ;; list of rooms in list index zero

  (map
   (lambda
     (set 'temp $0)

     (add-chr (player)
              (get temp 0) ;; x
              (get temp 1) ;; y
              'neutral
              (if (> (length temp) 3)
                  (get temp 3) ;; 1/0 possibly in this index if chr is replicant
                0))

     (if (> (length temp) 2)
         (chr-hp (player) (get temp 0) (get temp 1) (get temp 2))))
   (get $0 1)) ;; list of characters in list index one

  (set 'enemies-seen (get $0 2))
  (set 'frendlies-seen (get $0 3))

  (set 'last-zone (get $0 4)))
