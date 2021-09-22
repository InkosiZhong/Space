import os
import PIL
from PIL import Image, ImageDraw, ImageFont

text = '123abc'
im = Image.new('L', (300, 50), 255)
dr = ImageDraw.Draw(im)
#font = ImageFont.truetype(os.path.join("fonts". "simsun.ttc"), 18)

dr.text((20, 10), text)

im.show()