.global sys_exit
.global sys_fork
.global sys_sleep
.global sys_log

sys_exit:
    mov 4(%esp), %ebx
    mov $0, %eax
    int $0x80
    
    ret

sys_fork:
    mov 4(%esp), %ebx
    mov $1, %eax
    int $0x80
    
    ret

sys_sleep:
    mov 4(%esp), %ebx
    mov $2, %eax
    int $0x80
    
    ret
    
sys_log:
    mov 8(%esp), %ecx
    mov 4(%esp), %ebx
    mov $3, %eax
    int $0x80
    
    ret
