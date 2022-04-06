//ejercicio 8
xor ex, ex //acumula los valores
xor fx, fx //contador
mov dx, 1
mov cx, 1

//ciclo 1
primero: mov ax, %0001
sys 1
mov ax, [1]
jg 0, segundo //salta si es negativo
add fx, 1
add ex, ax
jmp primero

//ciclo 2
segundo: mov ax, %0001
vuelve: cmp fx, 1
jz finaliza
sys 1
sub ex, [1]
sub fx, 1
jmp vuelve

//finaliza
finaliza: mov [1], ex
sys 2
stop

