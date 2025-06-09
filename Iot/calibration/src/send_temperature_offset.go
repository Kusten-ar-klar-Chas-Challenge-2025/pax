package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"runtime"
	"strconv"

	"github.com/tarm/serial"
)

// Automatically detects the Arduino serial port based on OS
func findArduinoPort() ([]string, error) {
	var matches []string
	var err error

	switch runtime.GOOS {
	case "darwin": // macOS
		matches, err = filepath.Glob("/dev/tty.usbmodem*")
	case "linux":
		matches, err = filepath.Glob("/dev/ttyUSB*")
		if err == nil && len(matches) == 0 {
			matches, err = filepath.Glob("/dev/ttyACM*") // Some Linux distros use ttyACM
		}
	case "windows":
		matches = findWindowsCOMPorts() // Find COM ports without registry access
	default:
		return nil, fmt.Errorf("unsupported os: %s", runtime.GOOS)
	}

	if err != nil || len(matches) == 0 {
		return nil, fmt.Errorf("arduino not found")
	}

	return matches, nil // Return the first matching port
}

// Windows-specific function to list available COM ports
func findWindowsCOMPorts() []string {
	ports := []string{}
	for i := 1; i <= 256; i++ { // Windows supports COM1–COM256
		comPort := fmt.Sprintf("COM%d", i)
		ports = append(ports, comPort)
	}
	return ports
}

// Format temperature value
func formatTemperature(value float64) string {
	if value == 0 {
		return "TEMP0"
	} else if value > 0 {
		return fmt.Sprintf("TEMP+%.2f", value)
	} else {
		return fmt.Sprintf("TEMP%.2f", value) // Negative values already contain '-'
	}
}

func main() {

	// Print usage message
	if len(os.Args) < 2 || len(os.Args) > 3 {
		var executable string
		if runtime.GOOS == "windows" {
			executable = "send_temperature_offset.exe"
		} else {
			executable = "./send_temperature_offset"
		}
		fmt.Printf("Usage: %s <offset_value> [optional_serial_port]\n", executable)
		fmt.Println("--------------------------------------------------------------")
		fmt.Println("The program will autodetect and connect to the first available port")
		fmt.Printf("To display all available ports: %s --list\n", executable)

		return
	}

	// If user wants to list available ports
	if os.Args[1] == "--list" {
		ports, err := findArduinoPort()
		if err != nil {
			fmt.Println("Error:", err)
			return
		}
		fmt.Println("Available serial ports:")
		for _, port := range ports {
			fmt.Println("-", port)
		}
		return
	}

	calibrationValue := os.Args[1]
	parsedValue, err := strconv.ParseFloat(calibrationValue, 64)
	if err != nil || parsedValue < -50 || parsedValue > 50 {
		fmt.Println("Invalid input: No data sent")
		return
	}

	formattedValue := formatTemperature(parsedValue)

	var portName string
	// Use manual port if provided
	if len(os.Args) == 3 {
		portName = os.Args[2]
		fmt.Println("Using user-specified port:", portName)
	} else {
		// else automatically find the Arduino's serial port
		ports, err := findArduinoPort()
		if err != nil {
			fmt.Println("Error:", err)
			return
		}
		portName = ports[0] // Use first available port
	}

	// Connect to Arduino
	config := &serial.Config{Name: portName, Baud: 115200}
	port, err := serial.OpenPort(config)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	defer port.Close() // Ensure proper cleanup

	// Send data to Arduino
	_, err = port.Write([]byte(formattedValue + "\n"))
	if err != nil {
		fmt.Println("Write Error:", err)
	} else {
		fmt.Println("Sent calibration data:", formattedValue, "to", portName)
	}

	// Read only one line from the Arduino response
	reader := bufio.NewReader(port)
	response, err := reader.ReadString('\n')
	if err != nil {
		fmt.Println("Read Error:", err)
		return
	}

	fmt.Println("Arduino Response:", response)

}
