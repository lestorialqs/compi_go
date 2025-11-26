import os
import subprocess
import shutil
import re

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

def peephole_optimize(input_filepath, output_filepath):
    with open(input_filepath, 'r') as f:
        lines = f.readlines()

    optimized_lines = []
    i = 0

    while i < len(lines):
        line1 = lines[i].strip()
        
        # L1: movq %reg, %reg
        rr_match = re.search(r'^movq\s+(%[a-z0-9]+),\s*\1$', line1)
        if rr_match:
            i += 1 
            continue

        if i + 1 < len(lines):
            line2 = lines[i+1].strip()
            
            # L1: movq %rax, ADDR
            # L2: movq ADDR, %rax
            store_match = re.search(r'^movq\s+%rax,\s*(.+)$', line1)
            if store_match:
                address = store_match.group(1).strip()
                if '(' in address or not address.startswith('%'):
                    load_pattern = r'^movq\s+' + re.escape(address) + r',\s*%rax$'
                    
                    if re.search(load_pattern, line2):
                        optimized_lines.append(lines[i]) 
                        i += 2 
                        continue
            
            # L1: movq ADDR, %rax
            # L2: movq %rax, ADDR
            load_match = re.search(r'^movq\s*(.+),\s*%rax$', line1)
            if load_match:
                address = load_match.group(1).strip()
                if '(' in address or not address.startswith('%'):
                    store_pattern = r'^movq\s+%rax,\s*' + re.escape(address) + r'$'
                    
                    if re.search(store_pattern, line2):
                        optimized_lines.append(lines[i]) 
                        i += 2 
                        continue
            
            # L1: movq $IMM, %REG_SRC
            # L2: movq %REG_SRC, DEST  -> movq $IMM, DEST
            cmc1_match = re.search(r'^movq\s+\$([0-9]+),\s*(%[a-z0-9]+)$', line1)
            if cmc1_match:
                immediate = cmc1_match.group(1)
                reg_src = cmc1_match.group(2)
                
                cmc2_pattern = r'^movq\s+' + re.escape(reg_src) + r',\s*(.+)$'
                cmc2_match = re.search(cmc2_pattern, line2)
                
                if cmc2_match:
                    dest = cmc2_match.group(1)
                    
                    optimized_lines.append(f" movq ${immediate}, {dest}\n")
                    i += 2 
                    continue
                    
        optimized_lines.append(lines[i])
        i += 1
            
    with open(output_filepath, 'w') as f:
        f.writelines(optimized_lines)

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
    raw_asm_file = base_temp + ".s"
    if os.path.isfile(raw_asm_file):
        dest_raw = os.path.join(output_dir, f"input{i}.s")
        shutil.move(raw_asm_file, dest_raw)
        print(f"  ✓ Assembly RAW -> input{i}.s")

        dest_optimized = os.path.join(output_dir, f"input_optimized{i}.s")
        peephole_optimize(dest_raw, dest_optimized)
        print(f"  ✓ Assembly OPTIMIZED -> input_optimized{i}.s")
    
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