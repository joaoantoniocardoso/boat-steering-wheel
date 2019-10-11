EESchema Schematic File Version 4
LIBS:main-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4550 3500 1350 1250
U 5AABFC1B
F0 "CANBUS" 60
F1 "canbus.sch" 60
F2 "SI" I L 4550 3800 60 
F3 "SO" O L 4550 3700 60 
F4 "CS" I L 4550 3900 60 
F5 "SCK" I L 4550 3600 60 
F6 "INT" O L 4550 4000 60 
F7 "GND" O R 5900 3650 60 
F8 "VCC" I R 5900 3550 60 
F9 "CAN_H" B R 5900 3750 60 
F10 "CAN_L" B R 5900 3850 60 
$EndSheet
$Sheet
S 2850 3100 1150 1150
U 5AACA401
F0 "MCU" 60
F1 "atmega328p.sch" 60
F2 "SPI_MISO" I R 4000 3700 60 
F3 "SPI_MOSI" O R 4000 3800 60 
F4 "MCP_INT" I R 4000 4000 60 
F5 "VCC" I R 4000 3200 60 
F6 "GND" O R 4000 3300 60 
F7 "SPI_SCK" O R 4000 3600 50 
F8 "MCP_SS" O R 4000 3900 50 
$EndSheet
$Sheet
S 8250 3250 600  450 
U 5AAD16DD
F0 "CANBUS CONN" 60
F1 "canbus_connector.sch" 60
F2 "CAN_H" B L 8250 3500 60 
F3 "CAN_L" B L 8250 3600 60 
F4 "CAN_18V" O L 8250 3300 60 
F5 "CAN_GND" B L 8250 3400 60 
$EndSheet
Wire Wire Line
	4550 4000 4000 4000
Wire Wire Line
	4550 3900 4000 3900
Wire Wire Line
	4550 3600 4000 3600
Wire Wire Line
	4000 3800 4550 3800
Wire Wire Line
	8250 3500 7750 3500
Wire Wire Line
	7750 3500 7750 3750
Wire Wire Line
	7750 3750 5900 3750
Wire Wire Line
	5900 3850 7850 3850
Wire Wire Line
	7850 3850 7850 3600
Wire Wire Line
	7850 3600 8250 3600
Wire Wire Line
	6400 3550 5900 3550
$Comp
L Mechanical:MountingHole H104
U 1 1 5BE9AF15
P 5650 7000
F 0 "H104" H 5750 7050 50  0000 L CNN
F 1 "MountingHole" H 5750 7000 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 5650 7000 50  0001 C CNN
F 3 "~" H 5650 7000 50  0001 C CNN
	1    5650 7000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H103
U 1 1 5BE9BC53
P 5650 6800
F 0 "H103" H 5750 6850 50  0000 L CNN
F 1 "MountingHole" H 5750 6800 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 5650 6800 50  0001 C CNN
F 3 "~" H 5650 6800 50  0001 C CNN
	1    5650 6800
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H102
U 1 1 5BE9BC79
P 5650 6600
F 0 "H102" H 5750 6650 50  0000 L CNN
F 1 "MountingHole" H 5750 6600 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 5650 6600 50  0001 C CNN
F 3 "~" H 5650 6600 50  0001 C CNN
	1    5650 6600
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H101
U 1 1 5BE9BC9F
P 5650 6400
F 0 "H101" H 5750 6450 50  0000 L CNN
F 1 "MountingHole" H 5750 6400 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 5650 6400 50  0001 C CNN
F 3 "~" H 5650 6400 50  0001 C CNN
	1    5650 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3650 5900 3650
Wire Wire Line
	7650 3400 8250 3400
Wire Wire Line
	8250 3300 7650 3300
Wire Wire Line
	6400 3300 6400 3550
Wire Wire Line
	6500 3400 6500 3650
Wire Wire Line
	6600 3400 6500 3400
Wire Wire Line
	6600 3300 6400 3300
$Sheet
S 6600 3100 1050 500 
U 5AACFFEF
F0 "POWER SUPPLY" 60
F1 "supplies.sch" 60
F2 "+5V_OUT" O L 6600 3300 60 
F3 "+18V_IN" I R 7650 3300 60 
F4 "GND_IN" I R 7650 3400 50 
F5 "GND_OUT" O L 6600 3400 50 
F6 "+18V_OUT" I L 6600 3200 50 
$EndSheet
Wire Wire Line
	6400 3300 6400 3200
Wire Wire Line
	6400 3200 4000 3200
Connection ~ 6400 3300
Wire Wire Line
	6500 3400 6300 3400
Wire Wire Line
	6300 3400 6300 3300
Wire Wire Line
	6300 3300 4000 3300
Connection ~ 6500 3400
NoConn ~ 6600 3200
Wire Wire Line
	4550 3700 4000 3700
$EndSCHEMATC
