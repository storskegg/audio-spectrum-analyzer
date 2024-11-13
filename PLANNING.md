# Project Planning

## Milestones

1. [x] Fix the tooling
2. Get to know the board a bit
   1. Arduino Platform
   2. mbed OS (6.17.0)
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

**UPDATE:** 2024-11-11 Applied locally the [solution found in this commit](https://github.com/platformio/platform-ststm32/commit/62a48b6c8cd1a4a9fa79a808c7a6cdfbf728cd84)

## 2. Get to know the board a bit

I've done a ton of Arduino shit over the years, including on some pretty hot mcu's, but none like this. The GIGA 
sports an STM32H747. ST is known for some pretty wizardly hardware, and there'll be a learning curve associated with 
getting to know its features. Luckily, they have killer documentation.

One feature in particular I'm very interested in: DMA access to the ADCs' read registers. It exists, but I'm not 
sure if it's available outside STM32Cube's proprietary magic. Additionally, I need to know if DMA access is atomic, 
if there is an associated mutex to ensure I can't read in the middle of an active write, or if it's entirely 
unprotected.

### 2.1 Arduino Platform

The arduino platform provides basic wrappers around a more advanced API provided by the underlying OS, mbed. While 
it is still basic accessors, there's more going on on this board than I've used on other boards, so a bit of a 
learning curve, here, as I learn what's available.

### 2.2 mbed OS (6.17.0)

In the event I want more advanced access to the STM32H747's features, I may need to turn to what's exposed by mbed, 
an RTOS that is, unfortunately, slated for abandonment. (Zephyr looks to be the future replacement.) Anyways, it's 
worth better understanding what's available through the mbed API's directly.

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

## 5. ADC function - Basic

The goal of this step is to familiarize myself with ADC accessors, perform basic RMS readings from an external 
signal generator (on my oscilloscope), and generally get comfortable performing reads without blocking code 
execution, and just dumping values to Serial, verifying against my lab tooling.

## 6. UI Development II - Add ADC Control and Feedback

As with the DDS function, I'll need to move control and feedback to the UI from Serial. Start/Stop reads, display 
the read value, control RMS buckets, etc. It would be good to use the DDS function hooked up to the ADC loopback 
style so that we can adjust the frequency of the sine wave, and verify that the Vrms reads remain consistent, as 
well as supporting the continued effort of non-blocking everywhere possible.

## 7. ADC function - Advanced

This may get split into multiple sub-steps, as you'll read about below.

FFT time. We'll start with only a handful of bins...something easily spewable from the Serial line in ascii, or 
maybe CSV dump...doesn't really matter. Still, just a handful of bins, limited frequency range, test initially with 
the waveform generator on the oscilloscope, looking for harmonics--20 Hz is probably a good place to start, and keep 
and eye on 2nd, 3rd, 4th, and 5th order harmonics. Afterwards, use the DDS to supply the marker, also at 20 Hz, and 
compare the results to what's observable on the oscilloscope's FFT function. Ballpark correctness is all we need 
here at this time...just a sanity check.

Ugh...and decide on a windowing function, or choose to support a handful of them (e.g. hamming, hanning, etc).

Additionally, now is the time to support FFTs on sweep in addition to marker generation. A decision needs to be made 
whether to retain FFT data for each point in the sweep, or sum them as we go along, averaging in the end to produce 
a final verdict. Both might be good for different reasons...

- individual FFT retention is good for looking at the voltage spectrum for a given fundamental + harmonics...
  something i'm expressly interested in, since the DUT on my bench this moment is a valve device being operated 
  outside its linear-most bias regions, producing those nice even-order harmonics. additionally, this allows me to 
  better report on the gain of the DUT at each frequency (both in terms of fundamental, and %THD re: the harmonics)
- "lump average" after the sweep gives me spectrum power density, which is also useful for things like 
  characterizing an instrument (e.g. harmonica X through microphone Y), which allows me to better tune high/low pass 
  filters and tone stacks (essentially complicated pi-filters) on amplifiers, etc to give the best/desired compression, 
  tone, etc.

## 8. UI Development III - Add a Little FFT Display

Like most of these steps, there's some spike work to be done, here. This is where I incorporate an FFT display into 
the UI. Probably better to find than to write, so I'll be doing my research. At the same time, if I find multiple 
projects that I like, but they each have some features that I want that others lack, then I may wind up rolling my 
own. I don't want good enough, feature-wise. On that note, features...

On so many FFT displays, you get the rolling waterfall display, and I would definitely call that nice-to-have. It's 
not essential, but would be neat to see the spikes move across during a sweep. At the same time, if all goes well, 
the sweeps will be fast enough that it's not really work showing on a waterfall. Also, it's not the largest display, 
so UI real estate is valuable, and a waterfall would be the first thing to get cut in order to better display a 
layered line graph with axis labels and units, etc etc etc, not to mention the controls for the marker/sweep 
generator, etc.

I'll also want to switch between visibility modes: Voltage spectrum of fundamentals and spectrum power density being 
the two big ones. It should be easy to switch between modes.

There will likely be more noodling on this as time goes by.

## 9. UI Development IV - Functional, Practical UI Design

Up until this point, everything UI-related will have been super crufty, "good enough" for now design; and not 
practical for a finished product. So, I'm not aiming for distributable UI quality--it just has to work on my bench, 
so good enough for me, but that still means it needs to be practical. This is where I sit down, and design a 
polished UI, informed by my experiences with all of the prior UI steps.

800x480, and about...4in? ish? It's little. Fine for now.

## 10. UI Development V - Functional, Practical UI Implementation

Now that it's designed, get it into code.

## 11. Extended Features Consideration

There are a lot of feature ideas I've had just in the course of writing this document, let along the ones I thought 
of prior. These all fall into the categories of necessary complexities (to be solved after the core of it all has 
been done), nice-to-haves, etc.

What I need to do is start documenting them...here, I guess. Kind of a "tech radar" thing.
