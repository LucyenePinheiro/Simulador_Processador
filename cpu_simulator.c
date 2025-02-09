#include <stdio.h>
#include <stdlib.h>

#define RAM_SIZE 256
unsigned char RAM[RAM_SIZE];
int AX = 0, BX = 0, PC = 0, FLAGS = 0;
unsigned char IR = 0;
int last_result = 0;  // Armazenar o resultado da última operação

void print_state() {
    printf("PC: %d | IR: 0x%02X | AX: %d | BX: %d | FLAGS: %d\n", PC, IR, AX, BX, FLAGS);
}

void fetch() { IR = RAM[PC++]; }

void execute() {
    print_state();
    switch (IR) {
        case 0x01: AX = RAM[PC++]; last_result = AX; break;
        case 0x02: RAM[PC++] = AX; last_result = AX; break;
        case 0x03: AX += BX; last_result = AX; break;
        case 0x04: AX -= BX; last_result = AX; break;
        case 0x05: AX *= BX; last_result = AX; break;
        case 0x06: if (BX) { AX /= BX; last_result = AX; } else exit(1); break;
        case 0x07: PC = RAM[PC]; last_result = PC; break;
        case 0x08: FLAGS = (AX == BX) ? 1 : (AX > BX) ? 2 : 0; last_result = FLAGS; break;
        case 0xFF: exit(0);
        default: printf("Erro: Instrução inválida!\n"); exit(1);
    }
}

void run() { 
    while (1) { 
        fetch(); 
        execute(); 
    }
}

void load_program() {
    RAM[0] = 0x01; RAM[1] = 10;  // Exemplo: AX = 10
    RAM[2] = 0x01; RAM[3] = 20;  // Exemplo: BX = 20
    RAM[4] = 0x03; RAM[5] = 0x02; RAM[6] = 30;  // Exemplo de operação
    RAM[7] = 0xFF;  // Fim do programa
}

// Funções expostas para manipulação do estado
void start_cpu() {
    load_program();
    run();
}

void set_ax(int value) { AX = value; }
void set_bx(int value) { BX = value; }
void set_operation(int op) { IR = op; }  // Define a operação a ser executada

// Funções para obter os valores de AX, BX, PC, FLAGS, IR e o último resultado
int get_pc() { return PC; }
int get_ax() { return AX; }
int get_bx() { return BX; }
int get_flags() { return FLAGS; }
int get_ir() { return IR; }
int get_last_result() { return last_result; }  // Retorna o último resultado da operação
