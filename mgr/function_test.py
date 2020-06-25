import librosa.feature.spectral
import matplotlib.pyplot as plt
import numpy as np
import scipy as scipy
from scipy.io import wavfile
from scipy.signal import savgol_filter
import matplotlib.patches as mpatches

ver1, sr = librosa.load("big_sameFilters_ITD.wav")
ver2, sr_ = librosa.load("big_diffFilters_noITD.wav")
ver3, sr__ = librosa.load("big_sameFilters_noITD_lateralDiff.wav")
flatness = librosa.feature.spectral_flatness(y=ver1, n_fft = 2048)
flatness_ = librosa.feature.spectral_flatness(y=ver2, n_fft = 2048)
flatness__ = librosa.feature.spectral_flatness(y=ver3, n_fft = 2048)


flatness = np.transpose(flatness)
flatness_ = np.transpose(flatness_)
flatness__ = np.transpose(flatness__)

flatness = 10 * np.log10(flatness)
flatness_ = 10 * np.log10(flatness_)
flatness__ = 10 * np.log10(flatness__)

# flatness = flatness[0:490]
# flatness_ = flatness_[0:490]

freq = np.linspace(0.0, 1.0)

ones = [1] * len(flatness)
# a = ones - flatness
# b = ones - flatness_
# c = flatness - flatness_
# d = 0
# for i in range(0, len(flatness_)):
#     if flatness[i] > flatness_[i]:
#         d += 1
# print(flatness_.shape)
# print(d)
# plt.figure(6)
# plt.semilogy(a)
# plt.semilogy(b)
# plt.show()

time = list()
for i in range(0, len(flatness_)):
    time.append(i * 1024 / 44100)

plt.figure(1)
plt.plot(time, flatness, color='red', label="ITD")
# plt.xlabel('windows')
# plt.ylabel('flatness')
plt.show()

# plt.legend((version1_coh, version2_coh), ('label1', 'label2'))
plt.plot(time, flatness_, color='blue', label="Different channel filtering")
# plt.legend(loc='lower right')
# plt.xlabel('windows')
plt.ylabel('flatness')
plt.show()

plt.plot(time, flatness__, color='green', label="Difference in amplitude - first lateral reflections")
plt.legend(loc='higher right')
plt.xlabel('Time [s]')
plt.ylabel('Spectral flatness [dB]')
plt.show()


#########################################
sample_rate, version1 = wavfile.read("big_sameFilters_ITD.wav")
version1 = np.transpose(version1)
f, version1_coh = scipy.signal.coherence(version1[0], version1[1], sample_rate, nperseg=1024)
print(version1[0].shape, version1[1].shape)

sample_rate_, version2 = wavfile.read("big_diffFilters_noITD.wav")
version2 = np.transpose(version2)
f_, version2_coh = scipy.signal.coherence(version2[0], version2[1], sample_rate_, nperseg=1024)
print(version2[0].shape, version2[1].shape)

sample_rate_, version3 = wavfile.read("big_sameFilters_noITD_lateralDiff.wav")
version3 = np.transpose(version3)
f__, version3_coh = scipy.signal.coherence(version3[0], version3[1], sample_rate_, nperseg=1024)
print(version3[0].shape, version3[1].shape)

version1_coh = np.transpose(version1_coh)
version2_coh = np.transpose(version2_coh)
version3_coh = np.transpose(version3_coh)

# diff_filters_patch = mpatches.Patch(color='red', label='ITD')
# plt.legend(handles=[diff_filters_patch])
plt.figure(2)
plt.semilogy(f, version1_coh, color='red', label="ITD")
# plt.xlabel('frequency [Hz]')
# # plt.ylabel('Coherence')
plt.show()

# plt.legend((version1_coh, version2_coh), ('label1', 'label2'))
plt.semilogy(f_, version2_coh, color='blue', label="Different channel filtering")
# plt.legend(loc='lower left')
# plt.xlabel('frequency [Hz]')
# plt.ylabel('Coherence')
plt.show()

plt.semilogy(f__, version3_coh, color='green', label="Lateral reflections different channel amplitude")
plt.legend(loc='lower left')
plt.xlabel('frequency [Hz]')
plt.ylabel('Coherence')
plt.show()
# ITD_path = mpatches.Patch(color='red', label='ITD', loc='higher right')
# plt.legend(handles=[ITD_path])


###############################################################################################
# no_reverb = np.average(no_reverb[0], no_reverb[1])
f1, spectral_density_version1 = scipy.signal.welch(version1, sample_rate, nperseg=1024)
f1_, spectral_density_version2 = scipy.signal.welch(version2, sample_rate, nperseg=1024)
f1__, spectral_density_version3 = scipy.signal.welch(version3, sample_rate, nperseg=1024)

plt.figure(3)
data = np.array([spectral_density_version1[0], spectral_density_version1[1]])
data_ = np.average(data, axis=0)
data2 = np.array([spectral_density_version2[0], spectral_density_version2[1]])
data2_ = np.average(data2, axis=0)
data3 = np.array([spectral_density_version3[0], spectral_density_version3[1]])
data3_ = np.average(data3, axis=0)

data_ = 10 * np.log10(data_)
data2_ = 10 * np.log10(data2_)
data3_ = 10 * np.log10(data3_)

plt.plot(f1, data_, color='red', label="ITD")
# plt.xlabel('frequency [Hz]')
# plt.ylabel('Power Spectral Density')
plt.show()

# plt.semilogy(f1_, data2_, color='blue', label="Noised Reverberation Tail - Spectral Density")
plt.plot(f1_, data2_, color='blue', label="Different channel filtering")
# plt.legend(loc='lower left')
# plt.xlabel('frequency [Hz]')
# plt.ylabel('Power Spectral Density')
plt.show()

plt.plot(f1__, data3_, color='green', label="Difference in amplitude - first lateral reflections")
plt.legend(loc='higher left')
plt.xlabel('frequency [Hz]')
plt.ylabel('Power Spectral Density [dB]')
plt.show()


#########################
# hilbert_no_reverb = np.transpose(hilbert_no_reverb)
# hilbert_with_reverb = np.transpose(hilbert_with_reverb)

################################################################


data = np.array([version1[0], version1[1]])
data_ = np.average(data, axis=0)
data2 = np.array([version2[0], version2[1]])
data2_ = np.average(data2, axis=0)
data3 = np.array([version3[0], version3[1]])
data3_ = np.average(data3, axis=0)

hilbert_ver1 = scipy.signal.hilbert(data_)
hilbert_ver2 = scipy.signal.hilbert(data2_)
hilbert_ver3 = scipy.signal.hilbert(data3_)
hilbert_ver1_envelope = np.abs(hilbert_ver1)
hilbert_ver2_envelope = np.abs(hilbert_ver2)
hilbert_ver3_envelope = np.abs(hilbert_ver3)

# hilbert_ver1_envelope = np.abs(scipy.signal.hilbert(hilbert_ver1))
# hilbert_ver2_envelope = np.abs(scipy.signal.hilbert(hilbert_ver2))
# hilbert_ver3_envelope = np.abs(scipy.signal.hilbert(hilbert_ver3))

# hilbert_ver1_envelope = savgol_filter(hilbert_ver1_envelope, 51, 9)
# hilbert_ver2_envelope = savgol_filter(hilbert_ver2_envelope, 51, 9)
# hilbert_ver3_envelope = savgol_filter(hilbert_ver3_envelope, 51, 9)

instantaneous_phase = np.unwrap(np.angle(hilbert_ver1))
instantaneous_frequency = (np.diff(instantaneous_phase) / (2.0*np.pi) * sample_rate)
#
instantaneous_phase2 = np.unwrap(np.angle(hilbert_ver2))
instantaneous_frequency2 = (np.diff(instantaneous_phase2) / (2.0*np.pi) * sample_rate)

instantaneous_phase2 = np.unwrap(np.angle(hilbert_ver3))
instantaneous_frequency3 = (np.diff(instantaneous_phase2) / (2.0*np.pi) * sample_rate)



plt.figure(4)
plt.semilogy(hilbert_ver1_envelope, color='red', label="Interaural Time Difference")
plt.show()

# plt.semilogy(f1_, data2_, color='blue', label="Noised Reverberation Tail - Spectral Density")
plt.semilogy(hilbert_ver2_envelope, color='blue', label="Different channels filtering")
plt.legend(loc='lower left')
plt.show()

plt.semilogy(hilbert_ver3_envelope, color='green', label="Difference in amplitude - first lateral reflections")
plt.legend(loc='lower left')
plt.xlabel('Time [s]')
plt.ylabel('Signal Envelope ')
plt.show()





def plot():
    fig, axs = plt.subplots(2)
    fig.suptitle('flatness')
    axs[0].plot(flatness)
    axs[1].plot(flatness_)

    fig2, axs2 = plt.subplots(2)
    fig2.suptitle('coh')
    print(version2_coh.shape, version1_coh.shape)
    axs2[0].plot(version1_coh)
    axs2[1].plot(version2_coh)
    # print(len(hilbert_no_reverb[0][0:8820]))
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

    # fig3, axs3 = plt.subplots(2)
    # fig3.suptitle('spectral density')
    # # axs3[0].plot(f[0:100], spectral_density_no_reverb[0][0:100])
    # # axs3[1].plot(f_[0:100], spectral_density_with_reverb[0][0:100])
    # # print(len(hilbert_no_reverb[0][0:8820]))
    #
    # fig4, axs4 = plt.subplots(2)
    # fig4.suptitle('hilbert')
    # axs4[0].plot(hilbert_no_reverb_envelope)
    # axs4[1].plot(hilbert_with_reverb_envelope)
    #
    # fig5, axs5 = plt.subplots(2)
    # fig5.suptitle('channels')
    # axs5[0].plot(version2[0])
    # axs5[1].plot(version2[1])


# plot()
