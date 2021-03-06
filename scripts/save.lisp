;;;
;;; The game invokes this script when creating a save file. The result of this
;;; script will be stored in save memory, and passed to restore_save.lisp when
;;; reloading a game. As you see below, the list itself is an association list,
;;; and you may insert new key-value pairs if you have any data in particular
;;; that you'd like to save.
;;;


(list
 (cons 'save-protocol 1)
 (cons 'rooms (rooms (player)))
 (cons 'chrs (chrs (player)))
 (cons 'enemies-seen enemies-seen)
 (cons 'friendlies-seen friendlies-seen)
 (cons 'last-zone last-zone)
 (cons 'terrain (terrain (player))))
