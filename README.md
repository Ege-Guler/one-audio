# one-audio

## Overview
This application processes real-time audio input from a microphone, applies a filter to suppress specific unwanted frequencies (such as breathing noise), and routes the filtered audio to a virtual microphone device for playback or further use.

The application leverages ALSA (Advanced Linux Sound Architecture) for audio capture and playback and uses FFTW (Fastest Fourier Transform in the West) to process the audio in the frequency domain.

## Dependencies

### Libs

- **ALSA:** For capturing and playing back audio. Provides low-level access to audio devices using snd_pcm functions.

- **FFTW:** For performing Fast Fourier Transform (FFT) and Inverse Fast Fourier Transform (IFFT). Used for processing the audio signal in the frequency domain.

#### Install Dependencies: 
`sudo apt-get install libasound2-dev libfftw3-dev`