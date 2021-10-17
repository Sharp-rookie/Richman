from logging import fatal
from pathlib import Path
import subprocess, os, re
from time import sleep
from config import *
from compare import output_compare
from log import logger, logger_init

def scan_dir():
    path = Path(TEST_CASE_DIR)
    count = 0
    success = 0
    for dir in path.iterdir():
        if not dir.is_dir():
            logger.error(f'{dir} is not a dir')
        for f in dir.iterdir():
            if f.suffix == '.in':
                out = f.with_suffix('.out')
                count += 1
                success += check_file(f, out)
    logger.info(f"***测试通过：{success}/{count}***")
    logger.info(f"***测试失败：{count-success}/{count}***")

def check_file(infile: Path, outfile: Path) -> bool:
    try:
        logger.info(f'测试文件{infile}：')
        dumpfile = Path(DUMP_FILE_NAME)
        if dumpfile.exists():
            dumpfile.unlink()
        input = ''
        with infile.open('r+') as f:
            input: str = f.read()
            f.seek(0)
            if not input.startswith("1234\n"):
                f.write("1234\n1234\n")
        if QIMINGQIXIA == True:
            cmd = f"{GAME_EXE} < {infile}"
        else:
            cmd = f"{GAME_EXE} < {infile} 1>nul 2>&1"
        os.system(cmd)
        with infile.open('w') as f:
            f.write(input)
        if output_compare(Path(DUMP_FILE_NAME), outfile):
            logger.info(f'文件{infile}测试通过')
            return True
        else:
            logger.info(f'文件{infile}对应的输出{outfile}不一致')
        return False
    except FileNotFoundError:
        logger.error(f'文件不存在')
        return False
    except subprocess.CalledProcessError:
        logger.error(f'测试{infile}时调用游戏程序异常')
        return False

if __name__=='__main__':
    logger_init()
    scan_dir()
