;rdi - pPixelBuffer
;rsi - width
;rdx - height
;xmm0 - xmin
;xmm1 - xmax
;xmm2 - ymin
;xmm3 - ymax
section .text
    global mandelbrot

mandelbrot:
    push rbp
    mov rbp, rsp

    cvtsi2sd xmm4, rsi  ;xmm4 - szerokosc zmiennoprzecinkowy
    cvtsi2sd xmm5, rdx  ;xmm5 - wysokosc zmiennoprzecinkowy
    subsd xmm1, xmm0    ;xmm1 - (xmax - xmin)
    subsd xmm3, xmm2    ;xmm3 - (ymax - ymin)
    mov rbx, 0          ;rbx - numer piksela y
pixely_iter:
    mov rax, 0          ;rax - numer piksela x
pixelx_iter:
    mov dword[rdi], 0x000000
    cvtsi2sd xmm6, rax  ;xmm6 - numer piksela x zmiennoprzecinkowy
    mulsd xmm6, xmm1    ;wyskalowywanie
    divsd xmm6, xmm4
    addsd xmm6, xmm0    ;xmm6 = (x * (xmax - xmin))/width + xmin
    cvtsi2sd xmm7, rbx  ;xmm7 - numer piksela y zmiennoprzecinkowy
    mulsd xmm7, xmm3    ;wyskalowywanie
    divsd xmm7, xmm5
    addsd xmm7, xmm2    ;xmm7 = (y * (ymax - ymin))/width + ymin
    movsd xmm8, xmm6
    movsd xmm9, xmm7
    mov cl, 0
algortihm:
    movsd xmm10, xmm8
    movsd xmm11, xmm9
    mulsd xmm10, xmm10  ;xmm10 - x^2
    mulsd xmm11, xmm11  ;xmm11 - y^2
    mulsd xmm9, xmm8    ;xmm9 - x * y
    movsd xmm8, xmm10
    addsd xmm8, xmm11   ;xmm8 - x^2 + y^2
    mov r15, 4
    cvtsi2sd xmm12, r15
    comisd xmm8, xmm12
    ja paint
    inc cl
    cmp cl, 100
    je nopaint
    addsd xmm11, xmm11  ;xmm11 - 2 * y^2
    subsd xmm8, xmm11   ;xmm8 - x^2 - y^2
    addsd xmm8, xmm6    ;xmm8 - x^2 - y^2 + a
    addsd xmm9, xmm9    ;xmm9 - 2 * x * y
    addsd xmm9, xmm7    ;xmm9 - 2 * x * y + b
    jmp algortihm
paint:
    mov dword[rdi], 0xFFFFFF
nopaint:
    add rdi, 4
    inc rax
    cmp rax, rsi
    jl pixelx_iter
    inc rbx
    cmp rbx, rdx
    je end
    jmp pixely_iter
end:
    mov rsp, rbp
    pop rbp
    ret

