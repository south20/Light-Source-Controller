# Light Source Controller
This project is the result of the author's development and is not allowed for commercial use.
This project was developed using STM32F7 .
## Getting Started
AP Layer: Business Logic
The AP layer contains the system's business logic, responsible for processing commands from the host computer, managing system states, and implementing various functions. The following are the main functional modules of the AP layer:

AP_Get_system_info: Obtain system information
Ap_Get_system_ID(): Get system name, device ID
PE Layer: Functional Modules
The PE layer implements various functional modules, including motion control, sensor reading, etc. The following are the main functional modules of the PE layer:

PE_VA_motor.c
IO Layer: Hardware/Peripheral Management Module
The IO layer manages hardware and peripherals, providing the PE layer with low-level drivers and interfaces. The following are the main functional modules of the IO layer:

IO_VA_motor.c

<span id='Code Structure'/>

### 1. Code Structure 

```

├── AP Layer: Business Logic
│   ├── AP_Get_system_info()                #Obtain system information  
│   ├── Ap_Get_system_ID()                #Get system name, device
│   └── ...
├── PE Layer: Functional Modules
│   ├── PE_VA_motor.c
│   └── ...
├── IO Layer: Hardware/Peripheral Management Module
│   ├── IO_VA_motor.c
│   └── ...
└── README.md
```

## 2. Main Task and Interrupt Task
Main Task
The main task is the core logic of the system, constantly running in the main loop, handling the system's primary logic and business processes. The pseudocode structure of the main task is as follows:
```
// Main Task
void main_task() {
    // Initialization code...

    while (1) {
        // Process main system logic

        // Check for commands from the upper computer
        if (is_upper_computer_command_available()) {
            // Receive and parse command from the upper computer
            Command command = receive_upper_computer_command();

            // Process the command from the upper computer
            process_upper_computer_command(command);
        }

        // Other main task logic...
    }
}

```
Interrupt Task
The interrupt task is a part of the main task, responsible for handling various interrupt events in the system, including hardware interrupts and timer interrupts, etc. The pseudocode structure of the interrupt task is as follows:

```
// Interrupt Task
void interrupt_control_task() {
    // Handle various interrupt events, including hardware interrupts and timer interrupts, etc.
    // Specific interrupt handling functions depend on hardware and system requirements
}

```

## License Notices
This repository is under [BSD 3-Clause License](LICENSE.txt).
Light Source Controller is a research project intended for non-commercial use only. 
One must NOT use the code of Light Source Controller for any illegal, harmful, violent, racist, or sexual purposes. 
One is strictly prohibited from engaging in any activity that will potentially violate these guidelines.
Commercial use of this code is strictly prohibited without authorization from the authors.
