import os
import subprocess
import shutil

# Archivos c++
programa = ["main.cpp", "scanner.cpp", "token.cpp", "parser.cpp", "ast.cpp", "visitor.cpp"]

def clean_input(file_path):
    lines = []
    with open(file_path, 'r') as file:
        for line in file:
            line = line.rstrip()
            if "//" in line:
                line = line.split("//", 1)[0].rstrip()
            if line.strip() == "":
                lines.append("")
                continue
            lines.append(line)
    return "\n".join(lines)

# Compilar
compile = ["g++"] + programa
print("Compilando:", " ".join(compile))
result = subprocess.run(compile, capture_output=True, text=True)

if result.returncode != 0:
    print("Error en compilación:\n", result.stderr)
    exit(1)

print("Compilación exitosa\n")

# Ejecutar
input_dir = "inputs"
output_dir = "outputs"
os.makedirs(output_dir, exist_ok=True)

for i in range(1, 19):
    filename = f"input{i}.txt"
    filepath = os.path.join(input_dir, filename)

    if not os.path.isfile(filepath):
        continue

    print(f"{'='*50}")
    print(f"Procesando: {filename}")
    print(f"{'='*50}")

    # Limpiar input
    clean_filename = f"temp_input{i}.txt"
    clean_filepath = os.path.join(input_dir, clean_filename)
    
    input_lines = clean_input(filepath)
    with open(clean_filepath, 'w') as file:
        file.write(input_lines)

    # Ejecutar
    run_cmd = ["./a.out", clean_filepath]
    result = subprocess.run(run_cmd, capture_output=True, text=True)

    # Guardar stdout y stderr
    output_file = os.path.join(output_dir, f"output{i}.txt")
    with open(output_file, "w", encoding="utf-8") as f:
        f.write("=== STDOUT ===\n")
        f.write(result.stdout)
        f.write("\n=== STDERR ===\n")
        f.write(result.stderr)
        f.write(f"\n=== RETURN CODE: {result.returncode} ===\n")

    print(f"Output guardado en: output{i}.txt")

    # Mover archivos generados
    base_temp = os.path.join(input_dir, f"temp_input{i}")
    
    # 1. Tokens
    tokens_file = base_temp + "_tokens.txt"
    if os.path.isfile(tokens_file):
        dest = os.path.join(output_dir, f"tokens{i}.txt")
        shutil.move(tokens_file, dest)
        print(f"  ✓ Tokens -> tokens{i}.txt")
    
    # 2. Archivo .s (ensamblador)
    asm_file = base_temp + ".s"
    if os.path.isfile(asm_file):
        dest = os.path.join(output_dir, f"input{i}.s")
        shutil.move(asm_file, dest)
        print(f"  ✓ Assembly -> input{i}.s")
    
    # 3. Archivo de error (si existe)
    error_file = base_temp + "_error.txt"
    if os.path.isfile(error_file):
        dest = os.path.join(output_dir, f"error{i}.txt")
        shutil.move(error_file, dest)
        print(f"  ✗ Error -> error{i}.txt")

    # Limpiar archivo temporal
    if os.path.isfile(clean_filepath):
        os.remove(clean_filepath)

    print()

print(f"{'='*50}")
print("Procesamiento completado")
print(f"Archivos generados en: {output_dir}/")