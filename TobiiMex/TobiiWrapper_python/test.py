﻿import TobiiWrapper

#print(TobiiWrapper)
#help(TobiiWrapper)
#help(TobiiWrapper.wrapper)
#exit()



'''
Since samples are pulled in callbacks with the Tobii SDK, it may get
hiccups if your script is doing something very computationally heavy,
without allowing significant sleeps (which would allow the callback to
be called and therefore all samples to be collected appropriately).

This can be tested in a while-loop like the one below.                                    
'''
# Import modules
import pickle
import numpy as np
import time
from psychopy import core
import matplotlib.pyplot as plt

plt.close('all')

#%% ET settings
TobiiWrapper.wrapper.startLogging()
ets = TobiiWrapper.wrapper.findAllEyeTrackers()
print(ets)
if len(ets)==0:
    raise Exception('No eye trackers found')

tw = TobiiWrapper.wrapper(ets[0].address)
print(tw)

print(tw.getSDKVersion())
print(tw.getSystemTimestamp())

print(tw.connected_eye_tracker)
freq = tw.gaze_frequency
print(tw.gaze_frequency)
if freq==150:
    tw.gaze_frequency = 600
else:
    tw.gaze_frequency = 150
print(tw)
print(tw.tracking_mode)
print(tw.track_box)
print(tw.display_area)
print(tw.device_name)
   
#%% Record some data
success = tw.start('gaze')
success = tw.start('eyeImage')
success = tw.start('externalSignal')
success = tw.start('timeSync')
success = tw.start('positioning')
core.wait(0.2)
n_samples = tw.gaze_frequency * 2 # Record what should be two seconds of data

out = []
k = 0
ts = 0
ts_old = 0

t0 = time.clock()
while k < n_samples:
    samples = tw.peekN('gaze')
    if len(samples)>0:
        ts = samples[0].system_time_stamp

    if ts == ts_old:
        #core.wait(0.00001) # Wait 1/10 ms
        continue
   
    out.append([time.clock(), ts])
    k += 1
    ts_old = ts
   
print(time.clock() - t0)
success = tw.stop('gaze')
success = tw.stop('eyeImage')
success = tw.stop('externalSignal')
success = tw.stop('timeSync')
success = tw.stop('positioning')


#%% Plot data captured in real time (tobii time stamps, and loop intervals)
out = np.array(out)
plt.plot(np.diff(out[:, 0] * 1000))
plt.figure()
plt.plot(np.diff(out[:, 1] / 1000))

#%% Plot timestamps of samples in the buffer (and test pickle save and load)
all_samples = tw.peekN('gaze',10000000)
pickle.dump(all_samples,open( "save.pkl", "wb" ))
#print(all_samples[0])
#print(all_samples[0].left)
#print(all_samples[0].left.gaze_point.on_display_area.x)
#print(all_samples[0].left.gaze_point.on_display_area)
#print(all_samples[0].left.gaze_point)
#print(all_samples[0].left)
print(all_samples[0])
ut =[]
for i in all_samples:
    ut.append(i.system_time_stamp)
   
plt.figure()
plt.plot(np.diff(ut) / 1000)


all_samples2 = pickle.load( open( "save.pkl", "rb" ) )
ut2 =[]
for i in all_samples2:
    ut2.append(i.system_time_stamp)
   
plt.figure()
plt.plot(np.diff(ut2) / 1000)


all_images = tw.peekN('eyeImage',10000000)
print(all_images[0])
pickle.dump(all_images,open( "save2.pkl", "wb" ))

plt.figure()
plt.imshow(all_images[0].image)

all_images2 = pickle.load( open( "save2.pkl", "rb" ) )
plt.figure()
plt.imshow(all_images2[0].image)


all_ext = tw.peekN('externalSignal',10000000)
print(all_ext[0])
pickle.dump(all_ext,open( "save3.pkl", "wb" ))
all_ext2 = pickle.load( open( "save3.pkl", "rb" ) )
print(all_ext2[0])


all_t = tw.peekN('timeSync',10000000)
print(all_t[0])
pickle.dump(all_t,open( "save4.pkl", "wb" ))
all_t2 = pickle.load( open( "save4.pkl", "rb" ) )
print(all_t2[0])


all_p = tw.peekN('positioning',10000000)
print(all_p[0])
pickle.dump(all_p,open( "save5.pkl", "wb" ))
all_p2 = pickle.load( open( "save5.pkl", "rb" ) )
print(all_p2[0])

print('get log')
l=tw.getLog()
print('print log')
print(l)
tw.stopLogging()


plt.show()

