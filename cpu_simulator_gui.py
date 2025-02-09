import tkinter as tk
import ctypes

# Carregar a biblioteca C
cpu_simulator = ctypes.CDLL('./cpu_simulator.dll')  # Use o arquivo DLL no Windows

# Definir as funções da biblioteca C
cpu_simulator.start_cpu.argtypes = []
cpu_simulator.start_cpu.restype = None

cpu_simulator.set_ax.argtypes = [ctypes.c_int]
cpu_simulator.set_ax.restype = None

cpu_simulator.set_bx.argtypes = [ctypes.c_int]
cpu_simulator.set_bx.restype = None

cpu_simulator.set_operation.argtypes = [ctypes.c_int]
cpu_simulator.set_operation.restype = None

cpu_simulator.get_pc.restype = ctypes.c_int
cpu_simulator.get_ax.restype = ctypes.c_int
cpu_simulator.get_bx.restype = ctypes.c_int
cpu_simulator.get_flags.restype = ctypes.c_int
cpu_simulator.get_ir.restype = ctypes.c_int
cpu_simulator.get_last_result.restype = ctypes.c_int  # Função para pegar o último resultado

# Função para atualizar os labels na interface gráfica
def update_state():
    pc = cpu_simulator.get_pc()
    ax = cpu_simulator.get_ax()
    bx = cpu_simulator.get_bx()
    flags = cpu_simulator.get_flags()
    ir = cpu_simulator.get_ir()
    last_result = cpu_simulator.get_last_result()  # Pega o último resultado da operação

    label_pc.config(text=f"PC: {pc}")
    label_ax.config(text=f"AX: {ax}")
    label_bx.config(text=f"BX: {bx}")
    label_flags.config(text=f"FLAGS: {flags}")
    label_ir.config(text=f"IR: 0x{ir:02X}")
    label_result.config(text=f"Último Resultado: {last_result}")  # Exibe o resultado

# Função para iniciar a CPU
def start_cpu():
    cpu_simulator.start_cpu()
    update_state()

# Função para configurar o valor de AX
def set_ax():
    value = int(entry_ax.get())
    cpu_simulator.set_ax(value)
    update_state()

# Função para configurar o valor de BX
def set_bx():
    value = int(entry_bx.get())
    cpu_simulator.set_bx(value)
    update_state()

# Função para escolher a operação
def set_operation():
    op = int(operation_var.get())  # A operação é escolhida pelo usuário
    cpu_simulator.set_operation(op)
    update_state()

# Criar a janela da interface gráfica com Tkinter
window = tk.Tk()
window.title("Simulador de CPU")

# Frame para o estado da CPU
frame_state = tk.LabelFrame(window, text="Estado da CPU", padx=10, pady=10)
frame_state.grid(row=0, column=0, padx=10, pady=10, sticky="ew")

label_pc = tk.Label(frame_state, text="PC: 0")
label_pc.grid(row=0, column=0, sticky="w")

label_ir = tk.Label(frame_state, text="IR: 0x00")
label_ir.grid(row=1, column=0, sticky="w")

label_ax = tk.Label(frame_state, text="AX: 0")
label_ax.grid(row=2, column=0, sticky="w")

label_bx = tk.Label(frame_state, text="BX: 0")
label_bx.grid(row=3, column=0, sticky="w")

label_flags = tk.Label(frame_state, text="FLAGS: 0")
label_flags.grid(row=4, column=0, sticky="w")

label_result = tk.Label(frame_state, text="Último Resultado: 0")
label_result.grid(row=5, column=0, sticky="w")

# Frame para configurar AX e BX
frame_registers = tk.LabelFrame(window, text="Configurar Registradores", padx=10, pady=10)
frame_registers.grid(row=1, column=0, padx=10, pady=10, sticky="ew")

label_ax_input = tk.Label(frame_registers, text="Definir AX:")
label_ax_input.grid(row=0, column=0, sticky="w")

entry_ax = tk.Entry(frame_registers)
entry_ax.grid(row=0, column=1, sticky="ew")

set_ax_button = tk.Button(frame_registers, text="Definir AX", command=set_ax)
set_ax_button.grid(row=0, column=2, padx=5)

label_bx_input = tk.Label(frame_registers, text="Definir BX:")
label_bx_input.grid(row=1, column=0, sticky="w")

entry_bx = tk.Entry(frame_registers)
entry_bx.grid(row=1, column=1, sticky="ew")

set_bx_button = tk.Button(frame_registers, text="Definir BX", command=set_bx)
set_bx_button.grid(row=1, column=2, padx=5)

# Frame para escolher a operação
frame_operations = tk.LabelFrame(window, text="Escolher Operação", padx=10, pady=10)
frame_operations.grid(row=2, column=0, padx=10, pady=10, sticky="ew")

operation_var = tk.StringVar(value="1")  # Padrão é a operação 1

operation_choices = [
    ("Carregar AX", "1"),
    ("Armazenar AX em RAM", "2"),
    ("Somar (AX + BX)", "3"),
    ("Subtrair (AX - BX)", "4"),
    ("Multiplicar (AX * BX)", "5"),
    ("Dividir (AX / BX)", "6"),
    ("Pular para endereço", "7"),
    ("Comparar AX e BX", "8")
]

for i, (text, value) in enumerate(operation_choices):
    tk.Radiobutton(frame_operations, text=text, variable=operation_var, value=value).grid(row=i, column=0, sticky="w")

set_operation_button = tk.Button(frame_operations, text="Definir Operação", command=set_operation)
set_operation_button.grid(row=len(operation_choices), column=0, pady=5)

# Botão para iniciar a CPU
start_button = tk.Button(window, text="Iniciar CPU", command=start_cpu)
start_button.grid(row=3, column=0, padx=10, pady=10, sticky="ew")

# Executar a interface gráfica
window.mainloop()