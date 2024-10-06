
#include <iostream>
#include <fstream>
#include <vector>
#include "modules/audio_processing/include/audio_processing.h"


using namespace webrtc;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: <near_end.wav> <far_end.wav> <output.raw>" << endl;
        return 1;
    }
    ifstream inNearFile(argv[1], ios::binary);
    ifstream inFarFile(argv[2], ios::binary);
    ofstream outFile(argv[3], std::ios::binary);
    
    if (!(inNearFile.is_open() && inFarFile.is_open() && outFile.is_open())) {
        cerr << "Failed to open files" << endl;
        return 1;
    }
    // skip headers in wav files
    inNearFile.ignore(44);
    inFarFile.ignore(44);

    int kSampleRateHz = 16000;
    int samples_per_frame = kSampleRateHz / 100;
    int kNumChannels = 1;
    int bits_per_sample = 16;

    vector<short> render_frame(samples_per_frame);
    vector<short> capture_frame(samples_per_frame);
    vector<short> capture_frame_out(samples_per_frame);

    webrtc::StreamConfig in_config = webrtc::StreamConfig(kSampleRateHz, kNumChannels, false);
    webrtc::StreamConfig out_config = webrtc::StreamConfig(kSampleRateHz, kNumChannels, false);

    AudioProcessing* apm = AudioProcessingBuilder().Create();
    AudioProcessing::Config config;
    // check audio_processing.h for all the options
    config.echo_canceller.enabled = true;
    config.echo_canceller.mobile_mode = false;
    config.gain_controller1.enabled = true;
    config.gain_controller1.mode = AudioProcessing::Config::GainController1::kAdaptiveDigital;
    config.gain_controller2.enabled = true;
    config.noise_suppression.enabled = true;
    apm->ApplyConfig(config);

    while (!inNearFile.eof() && !inFarFile.eof()) {
        inNearFile.read(reinterpret_cast<char*>(capture_frame.data()), samples_per_frame * sizeof(short));
        inFarFile.read(reinterpret_cast<char*>(render_frame.data()), samples_per_frame * sizeof(short));
        if (inNearFile.gcount() / sizeof(short) < samples_per_frame || inFarFile.gcount() / sizeof(short) < samples_per_frame) {
            // ignore incomplete frame for simplicity
            break;
        }

        apm->ProcessReverseStream(render_frame.data(), in_config, out_config, NULL);
        apm->ProcessStream(capture_frame.data(), in_config, out_config, capture_frame_out.data());
        outFile.write(reinterpret_cast<const char*>(capture_frame_out.data()), samples_per_frame * sizeof(short));
    }
    delete apm;
    inNearFile.close();
    inFarFile.close();
    outFile.close();
    return 0;

}

