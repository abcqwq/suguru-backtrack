from bs4 import BeautifulSoup
import requests
import time

def getUrl(tc_num):
    tc_num = str(tc_num).zfill(3)
    # https://www.janko.at/Raetsel/Suguru/001.a.htm
    return f"https://www.janko.at/Raetsel/Suguru/{tc_num}.a.htm"

def generateTc(text, tc_num):
    """ example text
    begin
    puzzle suguru
    author Otto Janko
    solver Otto Janko
    source https://www.janko.at/Raetsel/
    info 4250-20-2081301140312-0-664
    unit 30
    size 6
    solver Otto Janko
    problem
    4 - - - - -
    - - - - - -
    - - 4 - - 1
    - - - 2 - -
    5 - - 3 5 -
    - - - - - -
    areas
    1 1 2 3 3 3
    1 2 2 2 3 4
    1 5 2 6 3 4
    5 5 6 6 6 4
    5 7 7 6 8 4
    5 8 8 8 8 4
    solution
    4 2 1 5 1 2
    3 5 3 2 4 5
    1 2 4 1 3 1
    4 3 5 2 4 2
    5 2 1 3 5 3
    1 3 4 2 1 4
    moves
    Z2;ce,1;be,2;ed,4;cd,5;dc,1;bd,3;cf,4;bf,3;df,2;ef,1;
    bc,2;ac,1;ab,3;ba,2;bb,5;cb,3;ca,1;db,2;ad,4;af,1;fa,2;
    ea,1;ff,4;fb,5;ec,3;fd,2;fe,3;eb,4;da,5;
    end
    """
    lines = text.split('\n')

    i = 0
    r, c = -1, -1
    for l in lines:
        if l.startswith('size'):
            boardSize = int(l.split()[1])
            r, c = boardSize, boardSize
            break
        elif l.startswith('rows'):
            r = int(l.split()[1])
        elif l.startswith('cols'):
            c = int(l.split()[1])
            break
        i += 1
    i += 2
    if 'problem' in lines[i]:
        i += 1
    problem = []
    for j in range(r): problem.append(lines[i].replace('-', '0').split()); i += 1
    i += 1
    areas = []
    for j in range(r): areas.append(list(map(int, lines[i].split()))); i += 1
    i += 1
    solution = []
    for j in range(r):
        tmp = lines[i].split()
        solution.append(tmp)
        i += 1

    # input
    with open(f'{tc_num}_tc.txt', 'w') as f:
        f.write(f'{r} {c}\n')
        for j in range(r):
            f.write(f'{" ".join(map(str, problem[j]))}\n')
        f.write(f'\n')
        for j in range(r):
            f.write(f'{" ".join(map(str, areas[j]))}\n')
    # answer
    with open(f'{tc_num}_sol.txt', 'w') as f:
        for j in range(r):
            f.write(f'{" ".join(solution[j])}\n')

start_tc, end_tc = 1, 180
for i in range(start_tc, end_tc + 1):
    print(f'Generating tc {i}..')
    url = getUrl(i)
    result = requests.get(url)
    doc = BeautifulSoup(result.text, "html.parser")
    infoText = doc.find_all('script')[2].text
    # print(infoText.strip())
    generateTc(infoText.strip(), i)
    time.sleep(0.1)