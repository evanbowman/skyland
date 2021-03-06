;;;
;;; The game invokes this script when loading a save file. The script must
;;; return a function, which receives and processes the save data.
;;;


(lambda
  (eval-other-file "/scripts/reset_hooks.lisp")

  (let ((data $0))
    (let ((load (lambda (cdr (assoc $0 data)))))

      (if (equal (load 'save-protocol) 1)
          (progn
            (terrain (player) (load 'terrain))

            (configure-player (player) (load 'rooms))

            (map
             (lambda
               (add-chr (player)
                        (get $0 0) ;; x
                        (get $0 1) ;; y
                        'neutral
                        (if (> (length $0) 3)
                            (get $0 3) ;; 1/0 possibly in this index if chr is replicant
                          0))

               (if (> (length $0) 2)
                   (chr-hp (player) (get $0 0) (get $0 1) (get $0 2))))
             (load 'chrs))

            (setq enemies-seen (load 'enemies-seen))
            (setq frendlies-seen (load 'friendlies-seen))

            (setq last-zone (load 'last-zone)))))))
