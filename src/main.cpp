#include "device/device.h"
#include <iostream>
#include <alsa/asoundlib.h>
#include <vector>
#include <csignal>
#include "audio/filter/filter.h"
#include <pthread.h>
#include <sched.h>

bool running = true; // Global flag to control the loop

//on sigint, control while loop
void signalHandler(int signum) {
    running = false;
}

void setRealtimePriority() {
    struct sched_param sched_param;
    sched_param.sched_priority = 99; // Max priority for real-time tasks
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sched_param) != 0) {
        std::cerr << "Failed to set real-time priority." << std::endl;
    }
}

int main(void)
{
    std::signal(SIGINT, signalHandler);
    size_t chunk_size = 512; // 16 is optimal but for fft I need smth between 256-1024


    // Common params
    unsigned int sample_rate = 44100;
    unsigned int channels = 1;

    /*
        the higher chunk_size the lower buffer_size 

        4096 - 16
        1024 - 512

        trying to figure out best settings for synchronization
    */

    snd_pcm_uframes_t buffer_size = 1024; // frames per period // former val4096
    snd_pcm_uframes_t period_size = 1024;

    std::vector<uint16_t> buffer(buffer_size * channels); // 16-bit little-endian buffer



    // Use plughw to auto resampling

    // Configure Physical PCM Device
    const std::string device_name = "hw:1,0";
    snd_pcm_stream_t stream_type = SND_PCM_STREAM_CAPTURE;

    // Configure Virtual PCM Device (loopback)
    const std::string virtual_device_name = "hw:2,0";
    snd_pcm_stream_t virtual_stream_type = SND_PCM_STREAM_PLAYBACK;
    
    Device *mic = new Device(stream_type, device_name, sample_rate, channels, buffer_size, period_size);
    Device *virtual_mic = new Device(virtual_stream_type, virtual_device_name, sample_rate, channels, buffer_size, period_size);


    auto init = mic->init();
    auto virtual_init = virtual_mic->init();

    if (!init || !virtual_init)
    {
        return -1;
    }
    
    setRealtimePriority();
    while (running)
    {
        int frames_read = mic->readData(buffer.data(), chunk_size);

        if(frames_read > 0){
            //Filter::volumeReduceFilter(buffer, frames_read);
            Filter::muffleBreathing(buffer, frames_read, chunk_size ,sample_rate);
            int frames_written = virtual_mic->writeData(buffer.data(), frames_read);
            if (frames_written != frames_read) {
                std::cerr << "w: " << frames_written << " r: " << frames_read << std::endl;
                std::cerr << "Mismatch between frames read and frames written!" << std::endl;
            }
        }
    }

    for (int data : buffer)
    {
        std::cout << data << " ";
    }

    std::cout << std::endl;

    delete mic;
    delete virtual_mic;


    return 0;
}