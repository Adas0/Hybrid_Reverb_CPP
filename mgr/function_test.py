import librosa.feature.spectral
import matplotlib.pyplot as plt
import numpy as np
import scipy as scipy
from scipy.io import wavfile


y, sr = librosa.load("no_reverb.wav")
y_, sr_ = librosa.load("with_reverb.wav")
flatness = librosa.feature.spectral_flatness(y=y)
flatness_ = librosa.feature.spectral_flatness(y=y_)

flatness = np.transpose(flatness)
flatness_ = np.transpose(flatness_)

# fig, axs = plt.subplots(2)
# fig.suptitle('flatness')
# axs[0].plot(flatness)
# axs[1].plot(flatness_)


sample_rate, no_reverb = wavfile.read("no_reverb.wav")
no_reverb = np.transpose(no_reverb)
f, no_reverb_coh = scipy.signal.coherence(no_reverb[0], no_reverb[1], sample_rate, nperseg=1024)
print(no_reverb[0].shape, no_reverb[1].shape)

sample_rate_, with_reverb = wavfile.read("with_reverb.wav")
with_reverb = np.transpose(with_reverb)
f_, with_reverb_coh = scipy.signal.coherence(with_reverb[0], with_reverb[1], sample_rate_, nperseg=1024)
print(with_reverb[0].shape, with_reverb[1].shape)

no_reverb_coh = np.transpose(no_reverb_coh)
with_reverb_coh = np.transpose(with_reverb_coh)



# fig, axs = plt.subplots(2)
# fig.suptitle('coherences')
#
# # axes = plt.gca()
# # axes.set_ylim([0, 2])
# axs[0].plot(f, no_reverb_coh)
# #
# # axes = plt.gca()
# # axes.set_ylim([0, 1])
# axs[1].plot(f_, with_reverb_coh)


# no_reverb = np.average(no_reverb[0], no_reverb[1])
f, spectral_density_no_reverb = scipy.signal.welch(no_reverb, sample_rate, nperseg=1024)
f_, spectral_density_with_reverb = scipy.signal.welch(with_reverb, sample_rate, nperseg=1024)

# hilbert_no_reverb = np.transpose(hilbert_no_reverb)
# hilbert_with_reverb = np.transpose(hilbert_with_reverb)

# fig2, axs2 = plt.subplots(2)
# fig2.suptitle('spectral density')
# axs2[0].plot(f[0:100], spectral_density_no_reverb[0][0:100])
# axs2[1].plot(f_[0:100], spectral_density_with_reverb[0][0:100])
# print(len(hilbert_no_reverb[0][0:8820]))

hilbert_no_reverb = scipy.signal.hilbert(no_reverb[0])
hilbert_with_reverb = scipy.signal.hilbert(with_reverb[0])
hilbert_no_reverb_envelope = np.abs(scipy.signal.hilbert(no_reverb[0]))
hilbert_with_reverb_envelope = np.abs(scipy.signal.hilbert(with_reverb[0]))
# instantaneous_phase = np.unwrap(np.angle(hilbert_no_reverb))
# instantaneous_frequency = (np.diff(instantaneous_phase) / (2.0*np.pi) * sample_rate)
#
# instantaneous_phase2 = np.unwrap(np.angle(hilbert_with_reverb))
# instantaneous_frequency2 = (np.diff(instantaneous_phase2) / (2.0*np.pi) * sample_rate)

fig3, axs = plt.subplots(2)
fig3.suptitle('hilbert')
axs[0].plot(hilbert_no_reverb_envelope)
axs[1].plot(hilbert_with_reverb_envelope)