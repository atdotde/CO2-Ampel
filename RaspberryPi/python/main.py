#!/usr/bin/python
# -*- coding:utf-8 -*-

import spidev as SPI
import ST7789
import time

from PIL  import Image
from PIL import ImageDraw
from PIL import ImageFont
from PIL import ImageColor

# Raspberry Pi pin configuration:
RST = 27
DC = 25
BL = 24
bus = 0 
device = 0 

# 240x240 display with hardware SPI:
disp = ST7789.ST7789(SPI.SpiDev(bus, device),RST, DC, BL)

# Initialize library.
disp.Init()

# Clear display.
disp.clear()

# Create blank image for drawing.
print("0.96inch LCD Module code")
image1 = Image.new("RGB", (disp.width, disp.height), "BLUE")
draw = ImageDraw.Draw(image1)
font = ImageFont.truetype('Font.ttf',30)
font10 = ImageFont.truetype('Font.ttf',12)
print ("***draw text")
draw.text((20, 0), u'微雪电子 ', font = font, fill = "WHITE")
draw.text((5, 40), 'This is the LCD test program', font = font10, fill = "WHITE")
disp.ShowImage(image1,0,0)
time.sleep(2)


print ("***draw line")
image2 = Image.new("RGB", (disp.width, disp.height), "WHITE")
draw = ImageDraw.Draw(image2)
draw.line([(0,0),(160,80)], fill = "BLUE",width = 5)
draw.line([(0,80),(160,0)], fill = "BLUE",width = 5)
draw.arc([(10,10),(70,70)],0,360,fill="BLUE")
draw.rectangle([(80,10),(150,70)],fill = "BLUE")
#draw.line([(100,10),(70,40)], fill = "BLUE",width = 5)
#draw.line([(40,200),(200,200)], fill = "BLUE",width = 5)
#draw.line([(200,20),(200,200)], fill = "BLUE",width = 5)
print ("***draw rectangle")
#draw.rectangle([(50,30),(190,70)],fill = "BLUE")
disp.ShowImage(image2,0,0)
time.sleep(2)


#draw.text((75, 110), '1.3inch LCD ', font = font10, fill = "BLUE")
#draw.text((72, 140), 'Test Program ', font = font10, fill = "BLUE")

#image1=image1.rotate(90)    #Rotation function




image = Image.open('pic.jpg')	
disp.ShowImage(image,0,0)
""""""