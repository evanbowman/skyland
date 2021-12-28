;;;
;;; neutral/2/1.lisp
;;;


(dialog "A damaged fortress floats into view. The residents do not respond to your radio signals.")


(init-opponent 5 'neutral)


(configure-player
 (opponent)
 '((power-core 3 13)
   (hull 2 13)
   (hull 2 14)
   (hull 3 12)
   (hull 4 12)))


(add-chr (opponent) 1 14 'neutral 0)


(setq after-converge-hook
      (lambda
        (dialog "You see a survivor amongst the wreckage. You cannot be sure whether the"
                " survivor is trustworthy. Invite survivor aboard?")

        (await-dialog-y/n)
        (setq after-converge-hook nil)))


(setq after-dialog-accepted-hook
      (lambda

        (setq temp (chr-slots (player)))
        (setq temp (get temp (choice (length temp))))


        (if temp
            (progn
              (rem-chr (opponent) 1 14)
              (if (equal (choice 2) 0)
                  (progn
                    (add-chr (player) (car temp) (cdr temp) 'neutral 0)
                    (dialog "The survivor joined your crew!")
                    (exit-level))
                (progn
                  (add-chr (player) (car temp) (cdr temp) 'hostile 0)
                  (dialog "The survivor turned out to be a vicious goblin!"))))
          (progn
            (dialog "Sadly, there's no room...")
            (exit-level)))))


(setq after-dialog-declined-hook
      (lambda
        ;; TODO...
        (exit-level)))
