# audio-spectrum-analyzer

Audio spectrum analyzer with marker/sweep generator.

## Design Considerations

### Feature: Waveform Generation

- **DDS** - Should produce sine waves at programmable frequencies throughout the audio spectrum, and beyond, at a 
  resolution <= 0.1 Hz 
- **PGA** - Should produce waveforms at a programmable amplitudes between 0.1 and 10 Vrms, and be free of DC bias.

### Feature: Data Acquisition

- **Impedance** - For the purposes of this device, it is expected that the DUT will be externally loaded (i.e. 50, 
  600, 1500, 15000, 1M ohm loads). As such, the acquisition input impedance should be as high as possible while 
  maintaining the lowest acceptable noise floor (think oscilloscope probes).
- **Attenuation** - The device should handle signals up to 10 Vrms. That is likely far stronger than the ADC's 
  maximum ratings (e.g. Vref of 3.3V). Because of this, there should be a stepped attenuator for signal range 
  selection, and some sort of crowbar circuit protection (e.g. diodes). Additionally, it should be noted that if the 
  signal is being divided, then the resulting error is the ADC's LSB multiplied by the voltage divider's quotient. 
  Explicitly, a 2:1 divider results in an error of `LSB * 2`, a 10:1 divider results in an error of `LSB * 10`.
- **Gain** - Related, if we're working with a smaller signal, but don't want to lose the resolution we have on a 
  "full-swing" signal, then we could add a PGA to bring the weak signal up to something that gives us more detail, 
  and knowing the gain we've added, we can adjust for it later in the processing and display.
- `LSB = Vref / (2^bits)` where bits is the ADC resolution.

### Feature: Data Processing & Reporting

- **RMS Samples** - This should be configurable, with a maximum to be determined after observing just how fast the 
  ADC will sample (probably dependent on how bad my code is `;-P`)
- **Sweep Modes** - Single sweep, continuous mode, etc
- **Display**
- **Exportable: CSV File**
- **Exportable: UDP Broadcast**

---

## Hardware Decisions

- Arduino GIGA R1 M7
    - **What:** One hell of a board boasting one hell of an mcu: an STM32H7xx beast of a controller.
    - **Why:** This thing has more than enough floating point capability for running the FFTs I'll be using. It also 
      has strong graphics capabilities, support for the display shield, an onboard, 16-bit ADC that isn't complete 
      trash, and that is already wired up to the  microphone ring of the onboard 1/8" audio TRRS jack. (I can add 
      better DAQ later.) 
- Arduino GIGA Display
    - **What:** It's all in the name
    - **Why:** High (enough) resolution display with touch support, and ready-made for the host controller (complete 
      with graphics acceleration, etc) 
- AD9833
    - **What:** Analog Devices DDS waveform generator.
    - **Why:** Ja, so the STM32H7xx mcu has a decent-enough DAC onboard that could be used for sweep/marker 
      generation, but I wanted a little more resolution, and more importantly I wanted to defer the actual DDS 
      function to dedicated hardware vs occupying precious cpu cycles to math/output sinusoidal waveforms.

