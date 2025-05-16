# STM32 with AD8495 Thermocouple Amplification  

This project demonstrates **high-precision temperature measurement** using a K-type thermocouple with the AD8495 amplifier and STM32's ADC, featuring cold-junction compensation and linearized output.  

---

## Hardware Requirements  
![AD8495 Circuit Diagram](ad8495_circuit.png)  
- **STM32F103C6 Microcontroller**  
- **AD8495 Thermocouple Amplifier**  
- **K-Type Thermocouple**  
- **2.5V Voltage Reference** (for ADC)  
- **3.3V Power Supply**  
- **100nF Decoupling Capacitors** (x2)  
- **Proteus 8.15+** (for simulation)  

---

## Circuit Overview  
### AD8495 Connections:  
- **VOUT** → PA0 (ADC1_IN0)  
- **+Vs** → 3.3V  
- **-Vs** → GND  
- **REF** → 2.5V (for cold-junction compensation)  
### STM32 Power:  
- **VDDA** → 2.5V (ADC reference)  
- **VDD** → 3.3V  

---

## Software Requirements  
- **STM32CubeMX** (for ADC configuration)  
- **STM32CubeIDE** (for firmware development)  
- **Proteus 8.15+** (simulation)  

---

## Configuration Steps  

### STM32CubeMX Setup  
1. **MCU Selection**: STM32F103C6 (8MHz clock)  
2. **ADC1 Configuration**:  
   - Channel: IN0 (PA0)  
   - Resolution: 12-bit  
   - External Trigger: Disabled  
   - Reference Voltage: 2.5V  
3. **Generate Code** in CubeIDE  

---

### STM32CubeIDE Implementation  
#### Key Functions:  
1. **Temperature Calculation**:  
    - float Read_Temperature(void) 
2. **Cold-Junction Compensation**:
    - float Apply_CJC(float temp)
3. **Main Loop**:
    - temp = Apply_CJC(Read_Temperature());
    - printf("Temperature: %.1f°C\r\n", temp);

### Proteus Simulation  
1. **Components**:  
    - STM32F103C6, AD8495, THERMOCOUPLE-K, Virtual Terminal
2. **Connections**:  
    - PA0 → AD8495 VOUT
    - 2.5V Reference → ADC VREF+
3. **Simulation**:  
   - Load `.hex` file  
   - Set thermocouple temperature in Proteus
   - Monitor output in terminal

## Troubleshooting  
- **Noisy Readings**:Add 10µF capacitor between VOUT and GND , Use shielded thermocouple wires
- **Negative Temperatures**: Ensure proper thermocouple polarity (red = negative)

## License  
**MIT License** — Free to use with attribution  