import os

def reformat_tc(file_name):
    with open(file_name, 'rb') as open_file:
        content = open_file.read()
    content = content.replace(b'\r\n\r\n', b'\r\n')
    with open(file_name, 'wb') as open_file:
        open_file.write(content)
    os.rename(file_name, file_name.replace('txt', 'in'))

def reformat_sol(file_name):
    os.rename(file_name, file_name.replace('txt', 'ans'))

path = '.'
tc_files_to_replace = [f for f in os.listdir(path) if 'tc' in f]
sol_files_to_replace = [f for f in os.listdir(path) if 'sol' in f]

for f in tc_files_to_replace:
    full_path = os.path.join(os.getcwd(), f)
    print(f'Found {f}, replacing...')
    if os.path.isfile(full_path): print('aman')
    reformat_tc(full_path)

for f in sol_files_to_replace:
    full_path = os.path.join(os.getcwd(), f)
    print(f'Found {f}, replacing...')
    if os.path.isfile(full_path): print('aman')
    reformat_sol(full_path)

print('Done')