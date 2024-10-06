# webrtc_audio_processing
Notes on how to build webrtc audio processing for windows.
Once library is built, there is a small demo how to use it in a cmake project.
Prebuilt libraries are released manually as releases of this repo.

1. Download the trimmed webrtc by pulseaudio:

```
curl -k -L -O http://freedesktop.org/software/pulseaudio/webrtc-audio-processing/webrtc-audio-processing-1.3.tar.gz
tar -zxvf webrtc-audio-processing-1.3.tar.gz
rm webrtc-audio-processing-1.3.tar.gz
```

2. Following suggestion from [here](https://gitlab.freedesktop.org/pulseaudio/webrtc-audio-processing/-/issues/2),
generate a visual studio solution. It requires meson, simply install it using `pip install`

```
cd webrtc-audio-processing-1.3
meson . build --buildtype=release -Db_vscrt=mt --backend vs --prefix $PWD/../win_webrtc
```

3. Open the `build/webrtc-audio-processing.sln` in Visual Studio and run "Build" -> "Build solution".
After the build, right click on "RUN_INSTALL" and "Build" it too. This will create a `win_webrtc` with libs and includes.

4. Open this directory in Visual Studio and build `echo_cancellation_demo.exe`

5. Test it, by running:

```bash
echo_cancellation_demo.exe near_end.wav far_end.wav output.raw
```

6. pack and upload to github as a release.
The latest one is: https://github.com/balacoon/webrtc_audio_processing/releases/download/1.3/win_webrtc.tar.gz
