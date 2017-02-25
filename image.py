from PIL import Image
import os

colors = [
	(0,0,0), (255,255,255), (255,0,0)
]

for filename in os.listdir('data'):
	lines = [line.rstrip('\n') for line in open('data/' + filename)]
	
	colorList = []
	
	for line in lines:
		for value in line:
			try:
				colorList.append(colors[int(value)])
			except:
				continue
	
	im = Image.new('RGB', (7, 7))
	im.putdata(colorList)
	sizedim = im.resize([700, 700])
	sizedim.save('images/' + filename + '.png')