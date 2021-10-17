from pathlib import Path
import subprocess, os, shutil
from config import *
from log import logger, logger_init

def scan_dir():
    path = Path(TEST_CASE_DIR)
    # WHITE_LIST = ['barrier']
    BLACK_LIST = []
    # BLACK_LIST = ['buy_tool', 'kuangdi', 'buyland', 'case_broke', 'case_gift', 'case_god_bless', 'case_rent_receive']
    for dir in path.iterdir():
        if not dir.is_dir():
            logger.error(f'{dir} is not a dir')
        if dir.name in BLACK_LIST:
            continue
        for f in dir.iterdir():
            if f.suffix == '.in':
                out = f.with_suffix('.out')
                gen_file(f, out)

def gen_file(infile: Path, outfile: Path) -> None:
    try:
        logger.debug(f'{infile}')
        input = ''
        with infile.open('r+') as f:
            input: str = f.read()
            f.seek(0)
            if not input.startswith("1234\n"):
                f.write("1234\n1000\n")
            f.write(input)
            f.write("\nquit\n")
        cmd = f"{GAME_EXE} < {infile} 1>nul 2>&1"
        # linux
        # cmd = f"cat {infile}"
        os.system(cmd)
        with infile.open('w') as f:
            f.write(input)
        shutil.copy(Path(DUMP_FILE_NAME), outfile)
        logger.info(f"生成文件 {outfile}")
        return
    except FileNotFoundError:
        logger.error(f'文件不存在')
        return
    except subprocess.CalledProcessError:
        logger.error(f'测试{infile}时调用游戏程序异常')
        return

if __name__=='__main__':
    logger_init()
    scan_dir()
