# Servo Motors Control with STM32  

This project demonstrates precise **PWM-based servo motor control** using an STM32 microcontroller, suitable for robotics and automation applications.  

---

## Hardware Requirements  
![Servo Control Circuit](servo_circuit.png)  
- **STM32F103C6 Microcontroller**  
- **SG90/MG996R Servo Motor**  
- **5V Power Supply** (for servo)  
- **3.3V Power Supply** (for STM32)  
- **Breadboard & Jumper Wires**  
- **Proteus 8.15+** (for simulation)  

---

## Circuit Overview  
### Servo Connections:  
- **Signal (Yellow/Orange)** → PA8 (TIM1_CH1)  
- **VCC (Red)** → 5V Supply  
- **GND (Brown)** → Common Ground  
### STM32 Power:  
- **3.3V** → VDD  
- **GND** → Common Ground  

---

## Software Requirements  
- **STM32CubeMX** (for PWM configuration)  
- **STM32CubeIDE** (for firmware development)  
- **Proteus 8.15+** (simulation)  

---

## Configuration Steps  

### STM32CubeMX Setup  
1. **MCU Selection**: STM32F103C6 (72MHz clock)  
2. **TIM1 Configuration**:  
   - Channel 1 (PA8): PWM Generation  
   - Prescaler: 71 (1MHz clock)  
   - Counter Period: 19999 (50Hz PWM frequency)  
   - Pulse: 1500 (1.5ms neutral position)  
3. **GPIO Configuration**:  
   - PA8 → Alternate Function (TIM1_CH1)  
4. **Generate Code** in CubeIDE  

---

### STM32CubeIDE Implementation  
#### Key Functions:  
1. **Servo Initialization**:  
    - void Servo_Init(void) 
2. **Set Servo Angle**:
    - uint16_t Read_ADC_Channel(uint8_t channel) 
3. **Main Loop**:
    - Set_Servo_Angle(0);   // 0°.
    - Set_Servo_Angle(90);   // 90°



### Proteus Simulation  
1. **Components**:  
    - STM32F103C6, SERVO-MOTOR, Virtual Terminal
2. **Connections**:  
    - PA8 → Servo Signal
    - 5V/GND → Servo Power
3. **Simulation**:  
   - Load `.hex` file  
   - Observe servo movement in 0°→90°→180° sequence

## Troubleshooting  
- **Servo Not Moving**:Verify 5V power to servo ,Check PWM signal with oscilloscope (expected: 50Hz, 0.5-2.5ms pulse)
- **Jittery Movement**: Add 100µF capacitor across servo power pins
- **Incorrect Angles**: Calibrate pulse width in Set_Servo_Angle()

## License  
**MIT License** — Free to use with attribution  
