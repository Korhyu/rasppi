//
// modifywav.c - Modify a WAV file using FFmpeg via pipes
// This example adds a crude tremolo effect to the audio
// Written by Ted Burke - last updated 10-2-2017
//
// To compile:
//
//    gcc modifywav.c -o modifywav -lm
//

#include <stdio.h>
#include <stdint.h>
#include <math.h>

void main()
{
    // Launch two instances of FFmpeg, one to read the original WAV
    // file and another to write the modified WAV file. In each case,
    // data passes between this program and FFmpeg through a pipe.
    FILE *pipein;
    FILE *pipeout;
    pipein  = popen("ffmpeg -i 12345678.wav -f s16le -ac 1 -", "r");
    pipeout = popen("ffmpeg -y -f s16le -ar 44100 -ac 1 -i - out.wav", "w");
    
    // Read, modify and write one sample at a time
    int16_t sample;
    int count, n=0;
    while(1)
    {
        count = fread(&sample, 2, 1, pipein); // read one 2-byte sample
        if (count != 1) break;
        ++n;
        sample = sample * sin(n * 5.0 * 2*M_PI / 44100.0);
        fwrite(&sample, 2, 1, pipeout);
    }
    
    // Close input and output pipes
    pclose(pipein);    
    pclose(pipeout);
}
