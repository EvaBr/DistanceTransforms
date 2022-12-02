#%%
import numpy as np
#from skimage.io import imread
from dists.dists import getGEO, getMBD
#from pathlib import Path 
import matplotlib.pyplot as plt
import time

def plotcheck(sls, imdict):
    #imdict={'GT':GT,'img':fat, 'geo':geo}
    #sls = 120
    #at the moment supports only slicing at dim 1
    plt.figure()
    l = len(imdict)
    for idd,(name,im) in enumerate(imdict.items()):
        if im.ndim>2:
            im = im[:,sls,...].squeeze()
        plt.subplot(1, l, idd+1)
        plt.imshow(im)
        plt.title(name)
        plt.axis('off')
    plt.show()

#%%

fat = np.load("500017_wat.npy")
S = fat.shape

#make artifical GT for now, 1 class/channel:
GT = np.zeros(S, dtype=np.uint8)
GT[160:170, 120, 130:140] = 1


#img should be in range 0-255 for using dists!
fat = ((fat-fat.min())/fat.max())*255


#%%
#OBS! both input arrays will be converted to uint8 for using dist.functs!!
#test in 3D:
start = time.time()
geo = getGEO(fat, GT).squeeze() #256 x 256 x 1 x 1
end = time.time()
print(f"Calculated GEOdt on size {fat.shape}: {(end-start):.1f} s")

start = time.time()
mbd = getMBD(fat, GT).squeeze() #256 x 256 x 1 x 1
end = time.time()
print(f"Calculated MBDdt on size {fat.shape}: {(end-start):.1f} s")


imdict = {'GT': GT, 'raw img': fat, 'GEO': geo, 'MBD': mbd}
plotcheck(120, imdict)



#%%
#test in 2D:
fim, fGT = fat[:, 120, :], GT[:, 120, :]
start = time.time()
fgeo = getGEO(fim, fGT).squeeze() #256 x 256 x 1 x 1
end = time.time()
print(f"Calculated GEOdt on size {fim.shape}: {(end-start)*1000:.3f} ms")

start = time.time()
fmbd = getMBD(fim, fGT).squeeze() #256 x 256 x 1 x 1
end = time.time()
print(f"Calculated MBDdt on size {fim.shape}: {(end-start)*1000:.3f} ms")


imdict = {'GT': fGT, 'raw img': fim, 'GEO': fgeo, 'MBD': fmbd}
plotcheck(120, imdict)






# %%
