# STM32 L298N Motor Driver: Dual H-Bridge for DC Motors  

This project demonstrates bidirectional control of **two DC motors** using an STM32 microcontroller and the L298N dual H-Bridge driver, featuring **PWM speed control** and **UART monitoring**.  

---

## Hardware Requirements  
![L298N Circuit Diagram](l298n_circuit.png)  
- **STM32F103C6 Microcontroller**  
- **L298N Dual H-Bridge Module**  
- **12V DC Motors (x2)**  
- **12V Power Supply** (for motors)  
- **3.3V Power Supply** (for STM32)  
- **Fast Recovery Diodes** (1N4007)  
- **Proteus 8.15+** (for simulation)  

---

## Circuit Overview  
### Motor Control Connections:  
- **ENA** → PA6 (PWM for Motor A via TIM3_CH1)  
- **IN1** → PB12 (Direction A1)  
- **IN2** → PB13 (Direction A2)  
- **ENB** → PA0 (PWM for Motor B via TIM3_CH3)  
- **IN3** → PA8 (Direction B1)  
- **IN4** → PA9 (Direction B2)  
### Power:  
- **12V** → L298N VCC  
- **5V** → STM32 VDD (if using onboard regulator)  
- **GND** → Common ground  

---

## Software Requirements  
- **STM32CubeMX** (for GPIO/TIMER/UART configuration)  
- **STM32CubeIDE** (for firmware development)  
- **Proteus 8.15+** (simulation)  

---

## Configuration Steps  

### STM32CubeMX Setup  
1. **MCU Selection**: STM32F103C6 (16MHz clock)  
2. **TIM3 Configuration**:  
   - Channel 1 (PA6) & Channel 3 (PA0): PWM Generation  
   - Prescaler: 0  
   - Counter Period: 65535  
   - PWM Mode: Mode 1  
3. **GPIO Configuration**:  
   - PB12, PB13, PA8, PA9 → Outputs (Direction control)  
4. **UART1 Configuration**:  
   - Baud Rate: 115200  
   - Word Length: 8 bits  
5. **Generate Code** in CubeIDE  

---

### STM32CubeIDE Implementation  
#### Key Functions:  
1. **Motor Direction Control**:  
    - void Set_Motor_Direction(uint8_t motor, bool forward)
2. **PWM Speed Adjustment:**:
    - void Set_Motor_Speed(uint8_t motor, uint8_t speed) 
3. **UART Status Feedback:**:
    - void Send_Motor_Status(void)

### Proteus Simulation  
1. **Components**:  
    - STM32F103C6, L298N, DC Motors (x2), Virtual Terminal
2. **Connections**:  
    - Match GPIO/PWM pins (PA0, PA6, PB12, PB13, PA8, PA9)
    - Connect UART1 TX to terminal
3. **Simulation**:  
   - Load `.hex` file  
   - Monitor motor speed/direction via terminal

## Troubleshooting  
- **Motors Not Moving**:Verify 12V power to L298N and 3.3V to STM32 ,Check ENA/ENB jumpers (remove for PWM control)
- **Erratic PWM**: Ensure TIM3 prescaler/counter settings match CubeMX
- **CUART Noise**: Confirm baud rate (115200) and ground connections


## License  
**MIT License** — Free to use with attribution  