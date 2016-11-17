#/usr/bin/env python3
from PIL import Image
from random import randint

def blend1():
    img = Image.new( 'RGBA', (512,512), "black")
    pixels = img.load()
    for i in range(img.size[0]):
        for j in range(img.size[1]):
            pixels[i,j] = (randint(1,255), randint(1,255), randint(1,255), 255);
	
   

    img.save("blend6.bmp")
    #img.show()

def blend2():
    d = 1024*10
    img = Image.new( 'RGBA', (d,d), "black")
    pixels = img.load()

    for i in range(img.size[0]):
        for j in range(img.size[1]):
            if (i < d/4 and j < d/4):
                pixels[i,j] = (120,120, 0, 255)
            elif (i < d/4 and j < d/2):
                pixels[i,j] = (120,0, 0, 255)
            elif (i <d/2 and j < d/4):
                pixels[i,j] = (80,80,80,255);
            else:
                pixels[i,j] = (255 * int(i / (d/2)), 255 *(1 - int(j / (d/2))), 0, 255)

    img.save("blend0.bmp")
    #img.show()

blend1()
#blend2()
