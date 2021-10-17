import logging
from config import *

logger = logging.getLogger("testlogger")

def logger_init():
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter(fmt='%(asctime)s - [%(levelname)s] - %(message)s',
                                datefmt='%Y/%m/%d %H:%M:%S')
    if LOG_USE_STREAM:
        s_handler = logging.StreamHandler()
        s_handler.setFormatter(formatter)
        logger.addHandler(s_handler)
    if LOG_USE_FILE:
        f_handler = logging.FileHandler(LOG_FILE, encoding='utf-8')
        f_handler.setFormatter(formatter)
        logger.addHandler(f_handler)
