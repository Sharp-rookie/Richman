import sys
from pathlib import Path

from log import logger

def read_file(filename: Path) -> 'list[str]':
    try:
        with filename.open('r', encoding='utf-8') as f:
            return f.readlines()
    except IOError:
        logger.error("没有找到文件:%s或读取文件失败！" % filename)
        sys.exit(1)

def check_list(lines: 'list[str]') -> 'list[str]':
    BLACK_LIST: 'list[str]' = ('', '\n', '\r\n')
    for i in BLACK_LIST:
        if i in lines:
            lines.remove(i)
    

def output_compare(out_file: Path, module_file: Path) -> bool:
    out = read_file(out_file)
    module = read_file(module_file)
    if(len(out) != len(module)):
        print('长度？')
        return False
    else:
        for i in range(len(module)):
            if(out[i].strip() == module[i].strip()):
                continue
            else:
                print(f"第{i}行？")
                return False
    return True