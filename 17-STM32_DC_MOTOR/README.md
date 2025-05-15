# STM32 H-Bridge Motor Driver: Control DC Motor Direction  

This project demonstrates bidirectional control of a DC motor using an STM32 microcontroller and an H-Bridge driver, featuring **PWM speed adjustment** and **UART feedback**.  

---

## Hardware Requirements  
![STM32 H-Bridge Circuit](hbridge_circuit.png)  
- **STM32F103C6 Microcontroller**  
- **L298N H-Bridge Driver** (or equivalent)  
- **12V DC Motor**  
- **12V Power Supply** (for motor)  
- **3.3V Power Supply** (for STM32)  
- **Fast Recovery Diodes** (1N4148 or 1N4007)  
- **Proteus 8.15+**  

---

## Circuit Overview  
- **H-Bridge Connections**:  
  - IN1 → PA4 (Forward Control)  
  - IN2 → PA5 (Reverse Control)  
  - ENA → PA2 (PWM Speed Control via TIM2)  
- **Motor Terminals**:  
  - OUT1/OUT2 → Motor positive/negative  
- **Protection**:  
  - Diodes across motor terminals (anti-back EMF)  
- **UART Debug**:  
  - PA9 (TX) → USB-UART RX (115200 baud)  

---

## Software Requirements  
- **STM32CubeMX** (for GPIO/TIMER/UART configuration)  
- **STM32CubeIDE** (for firmware development)  
- **Proteus 8.15+** (simulation)  

---

## Configuration Steps  

### STM32CubeMX Setup  
1. **MCU Selection**: STM32F103C6 (72MHz clock)  
2. **GPIO Configuration**:  
   - PA4, PA5 → Outputs (Motor direction control)  
   - PA2 → TIM2_CH1 (PWM for speed control)  
3. **TIM2 Configuration**:  
   - Mode: PWM Generation CH1  
   - Prescaler: 0  
   - Counter Period: 65535  
   - Pulse: 0 (initial duty cycle)  
4. **UART1 Configuration**:  
   - Baud Rate: 115200  
   - Word Length: 8 bits  
   - No Parity/Flow Control  
5. **Generate Code** in CubeIDE  

---

### STM32CubeIDE Implementation  
#### Key Functions:  
1. **Motor Direction Control**:  
    - void Set_Motor_Direction(bool forward) 
2. **PWM Speed Adjustment**:
    - void Set_Motor_Speed(uint8_t duty_cycle)
3. **UART Feedback**:
    - void Send_Status(char* message)
4. **Main Loop**:
    - Set_Motor_Direction(FORWARD);
    - Set_Motor_Speed(75);  // 75% speed
    - Send_Status("Motor: Forward @75%\r\n");

### Proteus Simulation  
1. **Components**:  
    - STM32F103C6, L298N, DC Motor, Virtual Terminal, Power Supplies
2. **Connections**:  
    - PA4 → L298N IN1, PA5 → IN2, PA2 → ENA
    - UART1 TX → Virtual Terminal RX
3. **Simulation**:  
   - Load `.hex` file  
   - Run simulation
   - Observe motor direction/speed changes in terminal logs

## Troubleshooting  
- **Motor Not Rotating**:Confirm H-Bridge power (12V) and STM32 3.3V supply and Check IN1/IN2 logic levels
- **PWM Not Working**: Verify TIM2 configuration (Prescaler=0, Counter Period=65535)
- **UART Garbage Output**: Ensure baud rate matches (115200)
- **Overheating H-Bridge**: Add heat sinks or reduce duty cycle

## License  
**MIT License** — Free to use with attribution  
