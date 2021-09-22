import os
import sys
import glob
import math
import random
import textwrap
from PIL import Image, ImageDraw, ImageFont

class TextDrawer:
    def __init__(self, home: str = './home', fonts: str = './fonts', out: str = './out') -> None:
        self.text = ''
        self.out_dir = out
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
        
        self.src = _get_all_path(home, 'txt')
        self.fonts = _get_all_path(fonts, 'ttc')

    
    def read_random_words(self, src) -> str:
        n_words = random.randint(50, 300)
        return src.read(n_words)

    def text2para(self, text: str) -> tuple:
        w = random.randint(20, 60)
        text = textwrap.fill(text, width=w)
        return text, w, math.ceil(len(text) / w)

    def draw_para(self) -> None:
        cnt = 0
        for src in self.src:
            with open(src, 'r', encoding='UTF-8') as f:
                while True:
                    text = self.read_random_words(f)
                    if not text:
                        break
                    text, w, h = self.text2para(text)
                    k = random.randint(12, 18)
                    im = Image.new('L', (int((1.1 * w) * k), int((1.6 * h) * k)), 255)
                    dr = ImageDraw.Draw(im)
                    idx = random.randint(0, len(self.fonts) - 1)
                    font = ImageFont.truetype(os.path.join(self.fonts[idx]), k)
                    dr.text((k, k), text, font=font)
                    im.save(os.path.join(self.out_dir, f'{"%05d"%cnt}.png'), 'png')
                    cnt += 1

if __name__ == '__main__':
    text_drawer = TextDrawer(sys.argv[1], sys.argv[2], sys.argv[3])
    text_drawer.draw_para()