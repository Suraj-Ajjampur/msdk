## Description

This example demonstrates how to take temperature readings from the MAX31889 using the I2C.

After initialization, a new reading is printed to the terminal every second.

## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analogdevicesinc.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

(None - this project builds as a standard example)

## Required Connections

If using the MAX32690EVKIT:
-   Connect a USB cable between the PC and the CN2 (USB/PWR) connector.
-   Install JP7(RX_EN) and JP8(TX_EN) headers.
-   Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.
-   Close jumper JP5 (LED1 EN).
-   Close jumper JP6 (LED2 EN).
-   You must connect P2.8 (SCL), P2.7 (SDA), VDD and GND to corresponding pins of MAX31889 EVKIT_A board (via J3 terminal).

If using the MAX32690FTHR:
-   Connect a USB cable between the PC and the J5 (USB/PWR) connector.
-   Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.
-   You must connect P2.8 (SCL), P2.7 (SDA), VDD and GND to corresponding pins of MAX31889 EVKIT_A board (via J1 header).

## Expected Output

```
****************** I2C SENSOR DEMO *******************

-->Temperature: 21.920000 °C
-->Temperature: 21.910000 °C
-->Temperature: 21.895000 °C
-->Temperature: 21.924999 °C
-->Temperature: 21.910000 °C
-->Temperature: 21.895000 °C
-->Temperature: 21.889999 °C
-->Temperature: 21.859999 °C
-->Temperature: 21.885000 °C
-->Temperature: 21.904999 °C
-->Temperature: 21.889999 °C
-->Temperature: 21.895000 °C

...
```

## Instructions for Using the MAX31889 Driver

### Opening MAX31889 Sensor Driver
```c
	mxc_i2c_sensor_driver_t MAX31889 = MAX31889_Open();
```



### Initializing the MAX31889
```c
	MAX31889.init(I2C_MASTER, MAX31889_I2C_SLAVE_ADDR0);
```
NOTE: This functions inializes the I2C peripheral used to communicate with the MAX31889, the I2C pins, and the MAX31889 itself.



### Taking a Temperature Reading
```c
	error = MAX31889.read(&temperature);
```


