#include <stdio.h>
#include <stdlib.h>

#define RAM_SIZE 256
unsigned char RAM[RAM_SIZE];
int AX = 0, BX = 0, PC = 0, FLAGS = 0;
unsigned char IR = 0;

void print_state() {
    printf("PC: %d | IR: 0x%02X | AX: %d | BX: %d | FLAGS: %d\n", PC, IR, AX, BX, FLAGS);
}

void fetch() { IR = RAM[PC++]; }

void execute() {
    print_state();
    switch (IR) {
        case 0x01: AX = RAM[PC++]; break;
        case 0x02: RAM[PC++] = AX; break;
        case 0x03: AX += BX; break;
        case 0x04: AX -= BX; break;
        case 0x05: AX *= BX; break;
        case 0x06: if (BX) AX /= BX; else exit(1); break;
        case 0x07: PC = RAM[PC]; break;
        case 0x08: FLAGS = (AX == BX) ? 1 : (AX > BX) ? 2 : 0; break;
        case 0xFF: exit(0);
        default: printf("Erro: Instrução inválida!\n"); exit(1);
    }
}

void run() { while (1) { fetch(); execute(); } }

void load_program() {
    RAM[0] = 0x01; RAM[1] = 10;
    RAM[2] = 0x01; RAM[3] = 20; BX = RAM[3];
    RAM[4] = 0x03; RAM[5] = 0x02; RAM[6] = 30;
    RAM[7] = 0xFF;
}

int main() {
    for (int i = 0; i < RAM_SIZE; i++) RAM[i] = 0;
    load_program();
    printf("Iniciando CPU...\n");
    run();
    return 0;
}




