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

            #if not (line.endswith("{") or line.endswith("}") or line.endswith(";")):
            #    line = line.rstrip() + ";"

            lines.append(line)

    return "\n".join(lines)

# Compilar
compile = ["g++"] + programa
print("Compilando:", " ".join(compile))
result = subprocess.run(compile, capture_output=True, text=True)

if result.returncode != 0:
    print("Error en compilación:\n", result.stderr)
    exit(1)

print("Compilación exitosa")

# Ejecutar
input_dir = "inputs"
output_dir = "outputs"
os.makedirs(output_dir, exist_ok=True)

for i in range(1, 19):
    filename = f"input{i}.txt"
    filepath = os.path.join(input_dir, filename)

    clean_filename = f"temp_input{i}.txt"
    clean_filepath = os.path.join(input_dir, clean_filename)

    if os.path.isfile(filepath):
        print(f"Limpiando {filename}")
        input_lines = clean_input(filepath)

        with open(clean_filepath, 'w') as file:
            file.write(input_lines)

    if os.path.isfile(clean_filepath):
        print(f"Ejecutando {clean_filename}")
        run_cmd = ["./a.out", clean_filepath]
        result = subprocess.run(run_cmd, capture_output=True, text=True)

        os.remove(clean_filepath)

        # Archivos generados
        tokens_file = os.path.join(input_dir, f"temp_input{i}.s")  # se crea en inputs/

        # Mover archivo de tokens si existe
        if os.path.isfile(tokens_file):
            dest_tokens = os.path.join(output_dir, f"input_{i}.s")
            shutil.move(tokens_file, dest_tokens)

    else:
        print(filename, "no encontrado en", input_dir)
