#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 256
#define NUM_REGISTERS 4

typedef struct {
    int registers[NUM_REGISTERS];  // AX, BX, PC, FLAGS
    int memory[MEMORY_SIZE];
    int ir;  // Registrador de instrução
} CPU;

CPU cpu;

void initialize_cpu() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu.registers[i] = 0;
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        cpu.memory[i] = 0;
    }
    cpu.ir = 0;
}

void print_state() {
    printf("%d\n", cpu.registers[2]);  // PC
    printf("%d\n", cpu.registers[0]);  // AX
    printf("%d\n", cpu.registers[1]);  // BX
    printf("%d\n", cpu.registers[3]);  // FLAGS
    printf("0x%02X\n", cpu.ir);        // IR
    printf("%d\n", cpu.registers[0]);  // Último resultado (AX)
}

void print_memory() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%02X ", cpu.memory[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
}

void execute_instruction(int instruction) {
    int opcode = (instruction >> 24) & 0xFF;
    int reg1 = (instruction >> 16) & 0xFF;
    int reg2 = (instruction >> 8) & 0xFF;
    int reg3 = instruction & 0xFF;

    switch (opcode) {
        case 0x01:  // ADD
            cpu.registers[reg1] = cpu.registers[reg2] + cpu.registers[reg3];
            break;
        case 0x02:  // SUB
            cpu.registers[reg1] = cpu.registers[reg2] - cpu.registers[reg3];
            break;
        case 0x03:  // MOV
            cpu.registers[reg1] = cpu.registers[reg2];
            break;
        case 0x04:  // LOAD
            cpu.registers[reg1] = cpu.memory[cpu.registers[reg2]];
            break;
        case 0x05:  // STORE
            cpu.memory[cpu.registers[reg1]] = cpu.registers[reg2];
            break;
        default:
            printf("Instrução desconhecida\n");
            break;
    }
    cpu.registers[2]++;  // Incrementa PC
}

int main(int argc, char *argv[]) {
    initialize_cpu();

    if (argc < 2) {
        printf("Uso: %s <comando>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "start") == 0) {
        initialize_cpu();
    } else if (strcmp(argv[1], "state") == 0) {
        print_state();
    } else if (strcmp(argv[1], "memory") == 0) {
        print_memory();
    } else if (strcmp(argv[1], "set_ax") == 0 && argc == 3) {
        cpu.registers[0] = atoi(argv[2]);
    } else if (strcmp(argv[1], "set_bx") == 0 && argc == 3) {
        cpu.registers[1] = atoi(argv[2]);
    } else if (strcmp(argv[1], "set_operation") == 0 && argc == 3) {
        cpu.ir = atoi(argv[2]);
    } else if (strcmp(argv[1], "run") == 0) {
        // Executa todas as instruções na memória
        while (cpu.registers[2] < MEMORY_SIZE) {
            int instruction = cpu.memory[cpu.registers[2]];
            if (instruction == 0) break;  // Fim das instruções
            execute_instruction(instruction);
        }
    } else {
        int instruction = strtol(argv[1], NULL, 16);
        execute_instruction(instruction);
    }

    return 0;
}