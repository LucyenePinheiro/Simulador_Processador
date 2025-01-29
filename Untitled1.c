#include <stdio.h>
#include <stdlib.h>

// Tamanho da RAM
#define RAM_SIZE 256

// Memória RAM
unsigned char RAM[RAM_SIZE];

// Registradores
int AX = 0, BX = 0, CX = 0, DX = 0; // Registradores de propósito geral
int PC = 0;                         // Contador de programa
unsigned char IR = 0;               // Registrador de instrução
int FLAGS = 0;                      // Registrador de flags (1 = zero, 2 = carry, etc.)

// Exibe o estado atual da CPU
void print_state() {
    printf("\n---------------- Estado da CPU -----------------\n");
    printf("PC: %d | IR: 0x%02X | FLAGS: %d\n", PC, IR, FLAGS);
    printf("AX: %d | BX: %d | CX: %d | DX: %d\n", AX, BX, CX, DX);
    printf("------------------------------------------------\n");
}

// Atualiza FLAGS após operações aritméticas
void update_flags(int result) {
    if (result == 0) {
        FLAGS = 1;  // Flag de zero
    } else {
        FLAGS = 0;  // Sem flag
    }
}

// Funções da ULA (Unidade Lógica e Aritmética)
int ula_add(int a, int b) {
    int result = a + b;
    update_flags(result);
    return result;
}

int ula_sub(int a, int b) {
    int result = a - b;
    update_flags(result);
    return result;
}

int ula_mul(int a, int b) {
    int result = a * b;
    update_flags(result);
    return result;
}

int ula_div(int a, int b) {
    if (b == 0) {
        printf("Erro: Divisão por zero!\n");
        exit(1);
    }
    int result = a / b;
    update_flags(result);
    return result;
}

// Busca a próxima instrução na RAM
void fetch() {
    if (PC >= RAM_SIZE) {
        printf("Erro: Tentativa de acessar endereço inválido na RAM (PC = %d)\n", PC);
        exit(1);
    }
    IR = RAM[PC]; // Busca a instrução
    PC++;         // Incrementa o contador de programa
}

// Decodifica e executa a instrução
void decode_and_execute() {
    print_state(); // Mostra o estado antes da execução da instrução

    switch (IR) {
        case 0x01: // LOAD: Carrega um valor da RAM para AX
            if (PC >= RAM_SIZE) {
                printf("Erro: Endereço inválido na RAM ao tentar LOAD (PC = %d)\n", PC);
                exit(1);
            }
            AX = RAM[PC];
            printf("Executando: LOAD AX, %d\n", AX);
            PC++;
            break;
        case 0x02: // STORE: Armazena o valor de AX na RAM
            if (PC >= RAM_SIZE) {
                printf("Erro: Endereço inválido na RAM ao tentar STORE (PC = %d)\n", PC);
                exit(1);
            }
            printf("Executando: STORE [%d] = %d\n", PC, AX);
            RAM[PC] = AX;
            PC++;
            break;
        case 0x03: // ADD: Soma AX e BX, armazena o resultado em AX
            printf("Executando: ADD AX, BX (%d + %d)\n", AX, BX);
            AX = ula_add(AX, BX);
            break;
        case 0x04: // SUB: Subtrai BX de AX, armazena o resultado em AX
            printf("Executando: SUB AX, BX (%d - %d)\n", AX, BX);
            AX = ula_sub(AX, BX);
            break;
        case 0x05: // MUL: Multiplica AX por BX, armazena o resultado em AX
            printf("Executando: MUL AX, BX (%d * %d)\n", AX, BX);
            AX = ula_mul(AX, BX);
            break;
        case 0x06: // DIV: Divide AX por BX, armazena o resultado em AX
            printf("Executando: DIV AX, BX (%d / %d)\n", AX, BX);
            AX = ula_div(AX, BX);
            break;
        case 0x07: // JMP: Salta para o endereço especificado
            if (PC >= RAM_SIZE) {
                printf("Erro: Endereço inválido na RAM ao tentar JMP (PC = %d)\n", PC);
                exit(1);
            }
            printf("Executando: JMP %d\n", RAM[PC]);
            PC = RAM[PC];
            break;
        case 0x08: // CMP: Compara AX e BX, define FLAGS
            printf("Executando: CMP AX, BX (%d vs %d)\n", AX, BX);
            if (AX == BX) {
                FLAGS = 1; // Flag de zero
            } else if (AX > BX) {
                FLAGS = 2; // Flag de maior
            } else {
                FLAGS = 0; // Sem flag
            }
            break;
        case 0xFF: // HLT: Para a execução
            printf("Executando: HLT - Programa encerrado.\n");
            exit(0);
        default:
            printf("Erro: Instrução desconhecida 0x%02X em PC = %d\n", IR, PC - 1);
            exit(1);
    }

    print_state(); // Mostra o estado após a execução da instrução
}

// Executa o programa
void run() {
    while (1) {
        fetch();
        decode_and_execute();
    }
}

// Carrega um programa de exemplo na RAM
void load_program() {
    // Programa de exemplo:
    // 1. LOAD 10 (carrega o valor 10 em AX)
    // 2. MOV BX, 20 (carrega o valor 20 em BX)
    // 3. ADD (soma AX e BX, resultado em AX)
    // 4. STORE 30 (armazena AX na posição 30 da RAM)
    // 5. HLT (para a execução)
    RAM[0] = 0x01; // LOAD AX
    RAM[1] = 10;   // Valor 10
    RAM[2] = 0x01; // LOAD BX (usando espaço temporário)
    RAM[3] = 20;   // Valor 20
    BX = RAM[3];   // MOV BX, 20
    RAM[4] = 0x03; // ADD
    RAM[5] = 0x02; // STORE AX na RAM[30]
    RAM[6] = 30;   // Endereço 30
    RAM[7] = 0xFF; // HLT
}

// Função principal
int main() {
    // Inicializa a RAM com zeros
    for (int i = 0; i < RAM_SIZE; i++) {
        RAM[i] = 0;
    }

    // Carrega o programa de exemplo na RAM
    load_program();

    // Executa o programa
    printf("Iniciando simulador de processador...\n");
    run();

    return 0;
}

