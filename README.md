# audio-spectrum-analyzer [TDR]

Audio spectrum analyzer with marker/sweep generator.

## Design Considerations

### Feature: _Waveform Generation_

Besides getting to know the chosen host controller, and there's a lot to learn, there, this will be the first thing I 
work on.

- **DDS** - Should produce sine waves at programmable frequencies throughout the audio spectrum, and beyond, at a 
  resolution <= 0.1 Hz 
- **PGA** - Should produce waveforms at a programmable amplitudes between 0.1 and 10 Vrms, and be free of DC bias.

### Feature: _Data Acquisition_

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

### Feature: _Data Processing & Reporting_

- **RMS Samples** - This should be configurable, with a maximum to be determined after observing just how fast the 
  ADC will sample (probably dependent on how bad my code is `;-P`)
- **Sweep Modes** - Single sweep, continuous mode, etc
- **Display**
- **Exportable: CSV File**
- **Exportable: UDP Broadcast**

---

## Hardware Decisions

- Arduino GIGA R1 M7
    - **What:** One hell of a board boasting one hell of an mcu: an STM32H747 beast of a controller.
    - **Why:** This thing has more than enough floating point capability for running the FFTs I'll be using. It also 
      has strong graphics capabilities, support for the display shield, an onboard, 16-bit ADC that isn't complete 
      trash, and that is already wired up to the  microphone ring of the onboard 1/8" audio TRRS jack. (I can add 
      better DAQ later.) 
- Arduino GIGA Display
    - **What:** It's all in the name
    - **Why:** High (enough) resolution display with touch support, and ready-made for the host controller (complete 
      with graphics acceleration, etc) 
- AD9833 (or similar)
    - **What:** Analog Devices DDS waveform generator.
    - **Why:** Ja, so the STM32H747 mcu has a decent-enough DAC onboard that could be used for sweep/marker 
      generation, but I wanted a little more resolution, and more importantly I wanted to defer the actual DDS 
      function to dedicated hardware vs occupying precious cpu cycles to math/output sinusoidal waveforms.

### Constraints

#### ADC

To start, I'll be using the host controller's onboard ADC. As stated above, it's actually not the crummy ADC 
typically found on MCU's. Using the onboard ADC also simplifies the start of this project.

- `Vref` Max: STM32H747's maximum `Vref` is `3.6V`. It's subject to strong transients, so the `Vref` needs a good 
  current source, and probably cap'd. The capacitor's time to recover from these transients affects the maximum 
  readable frequency into the ADC, but...it shouldn't really affect things too poorly in the audio spectrum. The 
  datasheet was mostly talking the MHz realm, which is solidly out of spec for this device. Regardless, the `Vref` 
  voltage must be very stable and accurate. Probably overkill, but I've thought about running an LM399 through a 
  voltage divider to half it, and then buffer the output with an ultra-low noise opamp that can handle the transients.
- `LSB = Vref / (2^bits)` where bits is the ADC resolution. The internal ADC is 16-bits, resulting in `~50uV`.
- While I'm using the onboard ADC, it'll be run single-ended. When I move to an external ADC, I'll consider a 
  differential ADC in conjunction with an instrumentation amp, or similar DAQ module to handle common mode noise 
  rejection, etc.

## References

### Datasheets

- STM32H747: https://www.st.com/resource/en/datasheet/stm32h747bi.pdf
- AD9833: https://www.mouser.com/datasheet/2/609/ad9833-3119795.pdf

### Application Notes

- STM32H747 16-bit ADC: https://www.st.com/resource/en/application_note/dm00628458-getting-started-with-the-stm32h7-series-mcu-16bit-adc-stmicroelectronics.pdf

