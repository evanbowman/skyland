;;
;; editor_keys.lisp
;;
;; Describes keyboard
;; shortcuts for our in-game
;; script editor.
;;

;; ---------------------------
;; All modes:
;; ---------------------------
;; Select: open/close text selection.

;; ---------------------------
;; nav-mode
;; ---------------------------
;; D-pad: move cursor
;; B: save and quit
;;
;; LBumper+A: insert newline
;; LBumper+B: backspace
;;
;; LBumper+Right-D-pad: jump to end of line
;; LBumper+Left-D-pad: jump to start of line
;; LBumper+Down-D-pad: jump to end of file
;; LBumper+Up-D-pad: jump to beginning of file
;;
;; RBumper+LBumper: center the view over the cursor
;; LBumper+RBumper: center the view over the cursor
;;
;; RBumper+Right-D-pad: cursor forwards one word
;; RBumper+Left-D-pad: cursor backwards one word
;; RBumper+Down-D-pad: cursor forwards one paragraph
;; RBumper+Up-D-pad: cursor backwards one paragraph
;;
;; RBumper+A: copy selection
;; RBumper+B: paste selection
;;
;; A: switch to edit-mode

;; ---------------------------
;; edit-mode
;; ---------------------------
;; B: back to nav-mode
;; D-pad: move keyboard cursor
;; A: insert character
;; LBumper: open autocomplete


;; ---------------------------
;; autocomplete-mode
;; ---------------------------
;; D-pad: move completion cursor
;; A: insert completion, switch to edit-mode
;; B: cancel, switch to edit mode
