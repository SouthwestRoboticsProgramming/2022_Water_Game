package com.swrobotics.watergame.driverstation;

import com.fazecast.jSerialComm.SerialPort;

public class TetheredConnection {
    private SerialPort port;

    public TetheredConnection() {
        for (SerialPort port : SerialPort.getCommPorts()) {
            if (port.getPortDescription().contains("USB Serial")) {
                this.port = port;
            }
        }

        port.setBaudRate(9600);
        port.setNumStopBits(1);
        port.setNumDataBits(8);
        port.setParity(SerialPort.NO_PARITY);
        if (!port.openPort())
            throw new RuntimeException("Failed to open serial port");
    }

    public void sendTest(boolean led) {
        byte[] data = new byte[4];
        data[0] = (byte) 0xA5;
        data[1] = 2;
        data[2] = 0x01;
        data[3] = (byte) (led ? 1 : 0);

        if (port.writeBytes(data, data.length) != data.length)
            throw new RuntimeException("Failed to write all data");
    }
}
