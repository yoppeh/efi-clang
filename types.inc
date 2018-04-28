; ------------------------------------------------------------------------------
; types.inc
;
; Data structures, macros, constants, etc. used by the other modules.
;


; for 32/64 portability and automatic natural align in structure definitions
struc int8 {
    .   db  ?
}
struc int16 {
    align 2
    .   dw  ?
}
struc int32 {
    align 4
    .   dd  ?
}
struc int64 {
    align 8
    .   dq  ?
}
struc intn {
    align 8
    .   dq  ?
}
struc dptr {
    align 8
    .   dq  ?
}


; helper macro for definition of relative structure member offsets
macro struct name
{
    virtual at 0
        name name
        sizeof.#name = $ - name
        name equ sizeof.#name
    end virtual
}
