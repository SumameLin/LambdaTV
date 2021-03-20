from PIL import Image
im = Image.open('./bottle.gif')
try:
  i = 0
  while True:
    im.seek(i)
    img_deal=im.resize((128,64),Image.ANTIALIAS)
    img_deal.convert('1')
    img_deal.save('gif2png/'+str(i)+'.bmp')
    i = i +1
except:
  pass
print('共拆解图像帧数'+str(i))