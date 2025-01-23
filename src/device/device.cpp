#include "device.h"


#include <iostream>

Device::Device( snd_pcm_stream_t stream_type,const std::string& device_name, unsigned int sample_rate, unsigned int channels, unsigned int buffer_size)
    : device_name_(device_name), stream_type_(stream_type), sample_rate_(sample_rate), channels_(channels), buffer_size_(buffer_size), pcm_handle_(nullptr) {}

Device::~Device() {
    close();
}

bool Device::init() {
    int err;

    if ((err = snd_pcm_open(&pcm_handle_, device_name_.c_str(), stream_type_, 0)) < 0) {
        setError("Failed to open PCM device", err);
        return false;
    }


    // set params to device
    snd_pcm_hw_params_t* hw_params;
    snd_pcm_hw_params_malloc(&hw_params);
    snd_pcm_hw_params_any(pcm_handle_, hw_params);
    snd_pcm_hw_params_set_access(pcm_handle_, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    
    // sample format  16 bit little-endian
    snd_pcm_hw_params_set_format(pcm_handle_, hw_params, SND_PCM_FORMAT_S16_LE);

    // sample rate
    snd_pcm_hw_params_set_rate_near(pcm_handle_, hw_params, &sample_rate_, nullptr);
    
    // 1 for mono, 2 for stereo
    snd_pcm_hw_params_set_channels(pcm_handle_, hw_params, channels_);

    if ((err = snd_pcm_hw_params(pcm_handle_, hw_params)) < 0) {
        setError("Failed to set hardware parameters", err);
        snd_pcm_hw_params_free(hw_params);
        return false;
    }

    snd_pcm_hw_params_free(hw_params);

    // Prepare the device to run
    if ((err = snd_pcm_prepare(pcm_handle_)) < 0) {
        setError("Failed to prepare PCM device", err);
        return false;
    }

    return true;
}

int Device::readData(void* buffer, size_t frames) {
    int frames_read = snd_pcm_readi(pcm_handle_, buffer, frames);
    if(frames_read == -EPIPE){
        setError("Buffer overrun", frames_read);
    }
    else if (frames_read < 0) {
        setError("Error reading from PCM device", frames_read);
    }
    return frames_read;
}

int Device::writeData(const void* buffer, size_t frames) {
    int err = snd_pcm_writei(pcm_handle_, buffer, frames);
    if (err < 0) {
        setError("Error writing to PCM device", err);
    }
    return err;
}

void Device::close() {
    if (pcm_handle_) {
        snd_pcm_close(pcm_handle_);
        pcm_handle_ = nullptr;
    }
}

std::string Device::getLastError() const {
    return last_error_;
}

void Device::setError(const std::string& message, int error_code) {
    last_error_ = message + ": " + snd_strerror(error_code);
    std::cerr << last_error_ << std::endl;
}
