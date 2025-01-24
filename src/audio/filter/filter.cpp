#include "filter.h"



void Filter::volumeReduceFilter(std::vector<uint16_t>& buffer, size_t frames){
    for (size_t i = 0; i < frames; ++i) {
        buffer[i] *= 100; 
    }
}

void Filter::muffleBreathing(std::vector<uint16_t>& buffer, size_t frames, unsigned int samples, unsigned int sample_rate){


    std::vector<double> in(samples, 0.0);
    std::transform(buffer.begin(), buffer.begin() + samples, in.begin(), [](uint16_t x) {return static_cast<double>(x);});

    for(int i = 0; i < 100; i++){
        std::cout << in[i] << " ";
    }
    

    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (samples / 2 + 1));
    fftw_plan forward_plan = fftw_plan_dft_r2c_1d(samples, in.data(), out, FFTW_ESTIMATE);

    fftw_execute(forward_plan);

    double breathLow = 20.0;  // Lower bound of breathing noise
    double breathHigh = 150.0; // Upper bound of breathing noise

    for (int i = 0; i < samples / 2 + 1; ++i) {
        double frequency = i * sample_rate / samples;

        if (frequency <= breathHigh && frequency >= breathLow) {
            double scale = 0.2 + (1.0 - 0.2) * (frequency / ((breathHigh + breathLow) / 2)); // Smooth scaling
            double magnitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
            double phase = atan2(out[i][1], out[i][0]);
            magnitude *= scale;
            out[i][0] = magnitude * cos(phase);
            out[i][1] = magnitude * sin(phase);
        }

    }


    // reconstruct sound
    std::vector<double> reconstructed_signal(samples);
    fftw_plan backward_plan = fftw_plan_dft_c2r_1d(samples, out, reconstructed_signal.data(), FFTW_ESTIMATE);

    // Perform IFFT
    fftw_execute(backward_plan);

    // Normalize the reconstructed signal
    for (int i = 0; i < samples; ++i) {
        reconstructed_signal[i] /= samples;
    }

    // copy to initial buffer and convert to unsigned int
    std::transform(reconstructed_signal.begin(), reconstructed_signal.begin() + samples, buffer.begin(), [](double x) {return static_cast<uint16_t>(x);});


    for(int i = 0; i < 100; i++){
        std::cout << buffer[i] << " ";;
    }


    fftw_destroy_plan(forward_plan);
    fftw_destroy_plan(backward_plan);
    fftw_free(out);
}