#include "device/device.h"
#include <iostream>
#include <alsa/asoundlib.h>
#include <vector>
#include <csignal>


bool running = true; // Global flag to control the loop

//on sigint, control while loop
void signalHandler(int signum) {
    running = false;
}


int main(void)
{
    std::signal(SIGINT, signalHandler);

    const std::string device_name = "default";
    snd_pcm_stream_t stream_type = SND_PCM_STREAM_CAPTURE;
    unsigned int sample_rate = 44100;
    unsigned int channels = 1;
    unsigned int buffer_size = 4096;                      // frames per period
    std::vector<uint16_t> buffer(buffer_size * channels); // 16-bit little-endian buffer

    Device *mic = new Device(stream_type, device_name, sample_rate, channels, buffer_size);

    auto init = mic->init();

    if (!init)
    {
        return -1;
    }

    while (running)
    {
        int frames_read = mic->readData(buffer.data(), buffer_size);
    }
    std::cout << "buffer";

    for (int data : buffer)
    {
        std::cout << data << " ";
    }

    std::cout << std::endl;

    return 0;
}