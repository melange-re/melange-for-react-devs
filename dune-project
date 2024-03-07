(lang dune 3.8)

; Use version 0.1 of the melange plugin for dune

(using melange 0.1)

; Set the name which is used by error messages

(name melange-for-react-devs)

; Enable cram tests

(cram enable)

; Copy expansion of alias into cram test sandbox

(expand_aliases_in_sandbox)
