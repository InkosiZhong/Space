import os
import sys
import glob
import math
import re
import random
import textwrap
from PIL import Image, ImageDraw, ImageFont
from chardet.universaldetector import UniversalDetector
import _thread

class TextDrawer:
    def __init__(self, home: str = './home', fonts: str = './fonts', out: str = './out') -> None:
        self.text = ''
        self.out_dir = out
        self.t_finish = None
        if os.path.exists(self.out_dir):
            print(f'output path \'{self.out_dir}\' already exists')
        else:
            os.mkdir(self.out_dir)

        def _get_all_path(dir, ext):
            if not os.path.exists(dir):
                print(f'\'{dir}\' does not exist')
                exit(-1)
            list = [os.path.join(dir, p) for p in glob.glob(os.path.join(dir, f'*.{ext}'))]
            if len(list) == 0:
                print(f'\'{dir}\' has no valid file')
                exit(-1)
            return list

        def _get_encode(srcs: list) -> list:
            encode = []
            for src in srcs:
                with open(src, 'rb') as f:
                    detector = UniversalDetector()
                    for line in f.readlines():
                        detector.feed(line)
                        if detector.done:
                            break
                    detector.close()
                encode.append((src, detector.result['encoding']))
                print(src, detector.result['encoding'])
            return encode
        
        self.src = _get_encode(_get_all_path(home, 'txt'))
        self.fonts = _get_all_path(fonts, 'ttc')

    
    def read_random_words(self, src) -> str:
        n_words = random.randint(10, 600)
        return src.read(n_words)

    def text2para(self, text: str) -> tuple:
        w = random.randint(min(len(text), 20), 60)
        text = textwrap.dedent(text)
        text = textwrap.fill(text, width=w)
        return text, w, math.ceil(len(text) / w)

    def draw_para(self, cycle: int = 1) -> None:

        def _contain_zh(para: str) -> bool:
            zh_patterns = re.compile(u'[\u4e00-\u9fa5]+')
            match = zh_patterns.search(para)
            return match

        def _process(self, i, src, encode, epoch):
            _, name = os.path.split(src)
            name = name.split('.')[0]
            print(f'thread[{i}] for {name} of epoch {epoch} started')
            with open(src, 'r', encoding=encode) as f:
                cnt = 0
                while True:
                    text = self.read_random_words(f)
                    if not text:
                        break
                    text, w, h = self.text2para(text)
                    k = random.randint(12, 25)
                    im = Image.new('L', (int((1.1 * w) * k // (1 if _contain_zh(text) else 2)), int((1.6 * h) * k)), 255)
                    dr = ImageDraw.Draw(im)
                    idx = random.randint(0, len(self.fonts) - 1)
                    font = ImageFont.truetype(os.path.join(self.fonts[idx]), k)
                    dr.text((k, k), text, font=font)
                    im.save(os.path.join(self.out_dir, f'{name}_{"%03d"%epoch}_{"%05d"%cnt}.png'), 'png')
                    cnt += 1
            print(f'thread for {name} of epoch {epoch} finished and generated {cnt} patches')
            self.t_finish[i] = True

        t_cnt = 0
        cycle = max(cycle, 1)
        self.t_finish = [False for _ in range(len(self.src) * cycle)]
        for epoch in range(cycle):
            for src, encode in self.src:
                try:
                    _thread.start_new_thread(_process, (self, t_cnt, src, encode, epoch))
                except:
                    print (f'Error: cannot start a new thread[{t_cnt}]')
                t_cnt += 1

    def done(self) -> bool:
        return False not in self.t_finish

if __name__ == '__main__':
    text_drawer = TextDrawer(sys.argv[1], sys.argv[2], sys.argv[3])
    if len(sys.argv) > 4:
        cycle = int(sys.argv[4])
    else:
        cycle = 1
    text_drawer.draw_para(cycle)
    while not text_drawer.done():
        pass
    print('all done')