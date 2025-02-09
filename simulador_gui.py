import tkinter as tk
from tkinter import ttk
import subprocess

# Função para atualizar os labels na interface gráfica
def update_state():
    result = subprocess.run(["./simulador", "state"], capture_output=True, text=True)
    output = result.stdout.splitlines()

    label_pc.config(text=f"PC: {output[0]}")
    label_ax.config(text=f"AX: {output[1]}")
    label_bx.config(text=f"BX: {output[2]}")
    label_flags.config(text=f"FLAGS: {output[3]}")
    label_ir.config(text=f"IR: {output[4]}")

    result_text.delete(1.0, tk.END)
    result_text.insert(tk.END, f"Último Resultado: {output[5]}\n")
    result_text.insert(tk.END, f"AX após operação: {output[1]}\n")
    result_text.insert(tk.END, f"BX após operação: {output[2]}\n")
    result_text.insert(tk.END, f"FLAGS após operação: {output[3]}\n")

    update_memory_view()

# Função para atualizar a visualização da memória
def update_memory_view():
    memory_text.delete(1.0, tk.END)
    result = subprocess.run(["./simulador", "memory"], capture_output=True, text=True)
    memory_text.insert(tk.END, result.stdout)

# Função para executar uma instrução por vez
def step():
    instruction = instruction_entry.get()
    if not instruction:
        print("Insira uma instrução!")
        return

    subprocess.run(["./simulador", instruction])
    update_state()

# Função para iniciar a CPU
def start_cpu():
    subprocess.run(["./simulador", "start"])
    update_state()

# Função para executar todas as instruções
def run_cpu():
    subprocess.run(["./simulador", "run"])
    update_state()

# Função para configurar o valor de AX
def set_ax():
    value = entry_ax.get()
    subprocess.run(["./simulador", f"set_ax {value}"])
    update_state()

# Função para configurar o valor de BX
def set_bx():
    value = entry_bx.get()
    subprocess.run(["./simulador", f"set_bx {value}"])
    update_state()

# Função para escolher a operação
def set_operation():
    op = operation_var.get()
    subprocess.run(["./simulador", f"set_operation {op}"])
    update_state()

# Criar a janela da interface gráfica com Tkinter
window = tk.Tk()
window.title("Simulador de CPU")

# Centralizar a janela na tela
window_width = 800
window_height = 600
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()
x = (screen_width // 2) - (window_width // 2)
y = (screen_height // 2) - (window_height // 2)
window.geometry(f"{window_width}x{window_height}+{x}+{y}")

# Frame para o estado da CPU
state_frame = ttk.LabelFrame(window, text="Estado da CPU", padding=10)
state_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")

label_pc = ttk.Label(state_frame, text="PC: 0")
label_pc.grid(row=0, column=0, sticky="w")

label_ir = ttk.Label(state_frame, text="IR: 0x00")
label_ir.grid(row=1, column=0, sticky="w")

label_ax = ttk.Label(state_frame, text="AX: 0")
label_ax.grid(row=2, column=0, sticky="w")

label_bx = ttk.Label(state_frame, text="BX: 0")
label_bx.grid(row=3, column=0, sticky="w")

label_flags = ttk.Label(state_frame, text="FLAGS: 0")
label_flags.grid(row=4, column=0, sticky="w")

# Frame para a memória RAM
memory_frame = ttk.LabelFrame(window, text="Memória RAM", padding=10)
memory_frame.grid(row=1, column=0, padx=10, pady=10, sticky="nsew")

memory_text = tk.Text(memory_frame, width=40, height=10)
memory_text.pack()

# Frame para o resultado do processamento
result_frame = ttk.LabelFrame(window, text="Resultado do Processamento", padding=10)
result_frame.grid(row=2, column=0, padx=10, pady=10, sticky="nsew")

result_text = tk.Text(result_frame, width=40, height=5)
result_text.pack()

# Frame para controles
control_frame = ttk.LabelFrame(window, text="Controles", padding=10)
control_frame.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")

start_button = ttk.Button(control_frame, text="Iniciar CPU", command=start_cpu)
start_button.grid(row=0, column=0, sticky="ew")

step_button = ttk.Button(control_frame, text="Executar Passo", command=step)
step_button.grid(row=1, column=0, sticky="ew")

run_button = ttk.Button(control_frame, text="Executar CPU", command=run_cpu)
run_button.grid(row=2, column=0, sticky="ew")

set_ax_button = ttk.Button(control_frame, text="Definir AX", command=set_ax)
set_ax_button.grid(row=3, column=0, sticky="ew")

set_bx_button = ttk.Button(control_frame, text="Definir BX", command=set_bx)
set_bx_button.grid(row=4, column=0, sticky="ew")

set_operation_button = ttk.Button(control_frame, text="Definir Operação", command=set_operation)
set_operation_button.grid(row=5, column=0, sticky="ew")

# Frame para escolha de operações
operation_frame = ttk.LabelFrame(window, text="Operações", padding=10)
operation_frame.grid(row=1, column=1, padx=10, pady=10, sticky="nsew")

operation_var = tk.StringVar(value="1")

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
    ttk.Radiobutton(operation_frame, text=text, variable=operation_var, value=value).grid(row=i, column=0, sticky="w")

# Configurar pesos das linhas e colunas para centralizar
window.grid_rowconfigure(0, weight=1)
window.grid_rowconfigure(1, weight=1)
window.grid_rowconfigure(2, weight=1)
window.grid_columnconfigure(0, weight=1)
window.grid_columnconfigure(1, weight=1)

# Executar a interface gráfica
window.mainloop()