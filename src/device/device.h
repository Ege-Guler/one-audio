#ifndef DEVICE_H
#define DEVICE_H

#include <alsa/asoundlib.h>
#include <string>

class Device
{
public:
    // most common settings
    Device(
        snd_pcm_stream_t stream_type, 
        const std::string &device_name = "default",
        unsigned int sample_rate = 44100,
        unsigned int channels = 1,
        snd_pcm_uframes_t buffer_size = 1024,
        snd_pcm_uframes_t period_size = 256)
        ;
    ~Device();
    bool init();
    int readData(void *buffer, size_t frames);
    int writeData(const void *buffer, size_t frames);
    void close();
    std::string getLastError() const;

private:
    std::string device_name_;      // ALSA device name
    snd_pcm_stream_t stream_type_; // Stream type (capture or playback)
    unsigned int sample_rate_;     // Sampling rate
    unsigned int channels_;        // Number of channels
    snd_pcm_uframes_t buffer_size_;     // Buffer size in frames
    snd_pcm_uframes_t period_size_;

    snd_pcm_t *pcm_handle_; // PCM device handle
    std::string last_error_;
    // Internal helper to log errors
    void setError(const std::string &message, int error_code);
};

#endif // DEVICE_H
