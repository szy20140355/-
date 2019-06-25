from PIL import Image
im = Image.open("./input.jpg")
width = im.width
height = im.height
print(height)
print(width)
for i in range(height):
    for j in range(width):
        print(im.getpixel((j,i)))