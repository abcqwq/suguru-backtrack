from subprocess import Popen, PIPE, STDOUT
from openpyxl.styles.borders import Border, Side
from openpyxl.styles import Alignment
from openpyxl import Workbook
import os

def write(row, col, val):
    ws.cell(row=row, column=col, value=val)

def initialize_excel(no_run=10):
    global wb, ws
    wb = Workbook()
    ws = wb.active
    write(1, 1, 'tc no')
    write(1, 2, 'puzzle size')
    for col in range(3, 3+no_run):
        write(1, col, 'run ' + str(col-2))
    write(1, 3+no_run, 'avg')

def save_excel():
    thin_border = Border(left=Side(style='thin'), right=Side(style='thin'), top=Side(style='thin'), bottom=Side(style='thin'))

    dims = {}
    for row in ws.rows:
        for cell in row:
            cell.alignment = Alignment(horizontal='center')
            cell.border = thin_border
            if cell.value:
                dims[cell.column_letter] = max((dims.get(cell.column_letter, 0), len(str(cell.value))))
    for col, value in dims.items():
        ws.column_dimensions[col].width = value
    wb.save('eval_result.xlsx')

def clean_stdout(stdout_data):
    output = stdout_data.decode().split('\r\n')
    output.pop()

    puzzle_size = output.pop(0)
    time_taken = float(output.pop())
    return output, puzzle_size, time_taken

def is_identic(config, sol):
    if len(config) != len(sol):
        return False
    for i in range(len(config)):
        if config[i] != sol[i]:
            return False
    return True

def solve(file_path, tc_no, no_run=10):
    write(tc_no+1, 1, tc_no)

    with open(file_path, 'rb') as open_file:
        tc = open_file.read()
    with open(file_path.replace('.in', '.ans'), 'rb') as open_file:
        sol = open_file.read().decode().split('\r\n')
        sol.pop()

    for _ in range(no_run):    
        p = Popen(['solver.exe'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
        stdout_data = p.communicate(input=tc)[0]
        config, puzzle_size, time_taken = clean_stdout(stdout_data)
        if _ == 0:
            write(tc_no+1, 2, puzzle_size)
        write(tc_no+1, 3+_, time_taken)
    
    write(tc_no+1, 3+no_run, '= AVERAGE(C{}:L{})'.format(tc_no+1, tc_no+1))
    
    if not is_identic(config, sol):
        print(f'Warning: testcase {file_path} might have multiple solutions.')


def main():
    initialize_excel()

    tc_dir = '../testcases/'
    tc_files = [f + '.in' for f in sorted([f.replace('.in', '') for f in os.listdir(tc_dir) if '.in' in f], key=int)]

    for file in tc_files:
        full_path = os.path.join(os.path.join(os.getcwd(), '..\\testcases\\'), file)
        if not os.path.isfile(full_path): print('o_o')
        solve(full_path, int(file.replace('.in', '')))

    save_excel()

if __name__ == "__main__":
    main()