# Project Planning

## Milestones

1. Fix the tooling
2. Get to know the board a bit
3. DDS function
4. UI Development I - DDS Control
5. ADC function - Basic
6. UI Development II - Add ADC Control and Feedback
7. ADC function - Advanced
8. UI Development III - Add a Little FFT Display
9. UI Development IV - Functional, Practical UI Design
10. UI Development V - Functional, Practical UI Implementation
11. Extended Features Consideration

## 1. Fix the tooling

Ugh. Ja, so I was getting started with this board using the Arduino IDE, and that took a shit on me 
tonight--suddenly it won't update any of its packages, and the processes hang and block anything else useful.

So, I started moving the project to PlatformIO sooner than I had intended...only to find out that PIO's `dfu-util` 
borks when it tries to upload an image to the controller. Issue searched on PIO's site, a comment added, and an 
issue opened on PIO's github.

I'll have to get one of these working tomorrow. In the meantime, documentation documentation documentation...

## 2. Get to know the board a bit

I've done a tone of Arduino shit over the years, including on some pretty hot mcu's, but none like this. The GIGA 
sports an STM32H747. STM is known for some pretty wizardly hardware, and there'll be a learning curve associate with 
getting to know its features. Luckily, they have killer documentation.

One feature in particular I'm very interested in: DMA access to the ADCs' read registers. It exists, but I'm not 
sure if it's available outside STM32Cube's proprietary magic. Additionally, I need to know if DMA access is atomic, 
if there is an associated mutex to ensure I can't read in the middle of an active write, or if it's entirely 
unprotected.

## 3. DDS function

I've got the AD9833 board right here. I am curious about the quality of its oscillator, so I'll hook that up to the 
oscilloscope. There's already a nice lib for the board, so it's just a matter of starting with hardcoded settings, 
and then adjusting them via serial, making it interactive.

It should support Marker (single frequency), Sweep, and 2-Tone functionality. At this point, Sweep will have an 
artificial latency between changing frequencies, but it _will_ sweep based on logarithmic Fourier frequencies 
(already written).

2-Tone functionality will require a mixer installed on the TRRS jack, mixing L and R channels. Hmm...now that I'm 
thinking of it...I _think_ there's an audio amp associated with the TRRS...I'll need to check into that.

Additionally, I'll need to verify the output voltage on ye olde 34401A at various frequencies through the desired 
range, and test them unloaded, as well as loaded at some common, sane values (1M, 150k, 50k, 1k, and 600R).

I'll choose at this time whether I need to add some programmable gain right off, or maybe just add unity gain 
current buffering in the form of a JFET source follower, or with something like a TL072.

## 4. UI Development I - DDS Control

I have a few options re: library packages, and will have to play with each of them to figure out what I like and 
dislike about each of them. This is a great time to make a simple UI to control the DDS functions!

The UI should never be blocked. Use IRQ for touch response.

1. Start with integer value frequencies, and simple up/down arrows, with an updating display of current setting.
2. Add numerical entry (dialog, kinda like the oscilloscope?), and switch to doubles. The AD9833 clocked at 25MHz 
   supports a resolution of 0.1 Hz, so that's all the precision we need. Also...that's really all I need for audio 
   purposes, anyways. (Why doubles, when? I prefer to minimize type casting whenever possible.)
