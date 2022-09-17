package com.swrobotics.watergame.driverstation;

import com.fazecast.jSerialComm.SerialPort;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public final class NetworkedConnection {
    private static final int MAX_PACKET_LEN = 64;
    private static final byte START_BYTE = (byte) 0xA5;

    private static final byte PACKET_IN_PONG = 0x01;
    private static final byte PACKET_IN_HANDSHAKE = 0x02;
    private static final byte PACKET_IN_LOG = 0x03;

    private static final byte PACKET_OUT_PING = 0x01;
    private static final byte PACKET_OUT_STATE = 0x02;
    private static final byte PACKET_OUT_CONTROLS = 0x03;
    private static final byte PACKET_OUT_INIT_WIFI_UPLOAD = 0x04;

    private static final int WIFI_UPLOAD_MAGIC = 0x7ABC045A;

    private final InputStream in;
    private final OutputStream out;

    private final byte[] readBuf = new byte[MAX_PACKET_LEN];
    private int readPtr;
    private int readLen;

    private enum ReadState {
        AWAIT_START,
        READ_LENGTH,
        READ_CONTENT
    }
    private ReadState readState = ReadState.AWAIT_START;

    public NetworkedConnection() {
        try {
            Socket socket = new Socket("192.168.4.1", 80);
            in = socket.getInputStream();
            out = socket.getOutputStream();
        } catch (IOException e) {
            throw new RuntimeException("Connection failed", e);
        }
    }

    private void handlePacketInPong() {
        /*
         byte[] payload = new byte[8];
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            long mask = 0xFFL << shift;

            payload[i] = (byte) ((time & mask) >> shift);
        }

         */

        long payload = 0;
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            payload |= (long) (readBuf[i + 1] & 0xFF) << shift;
        }

        long latency = System.currentTimeMillis() - payload;

        System.out.println("Pong: latency " + latency + " ms");
    }

    private void handlePacketInLog() {
        char[] strData = new char[32];

        int i = 1;
        while (readBuf[i] != 0) {
            strData[i - 1] = (char) readBuf[i];
            i++;
        }
        String str = new String(strData);

        System.out.println("Log: " + str);
    }

    private void handlePacket() {
        byte id = readBuf[0];
        switch (id) {
            case PACKET_IN_PONG:
                handlePacketInPong();
                break;
            case PACKET_IN_HANDSHAKE:
                System.out.println("Handshake");
                break;
            case PACKET_IN_LOG:
                handlePacketInLog();
                break;
            default:
                System.out.println("Unknown id " + id);
        }
    }

    public void read() {
        try {
            while (in.available() > 0) {

                int read = in.read();
                if (read == -1)
                    throw new IOException();
                byte b = (byte) read;

                switch (readState) {
                    case AWAIT_START:
                        if (b == START_BYTE)
                            readState = ReadState.READ_LENGTH;
                        break;
                    case READ_LENGTH:
                        readLen = b;
                        if (readLen > MAX_PACKET_LEN)
                            throw new RuntimeException("Invalid packet length");
                        readPtr = 0;
                        readState = ReadState.READ_CONTENT;
                        break;
                    case READ_CONTENT:
                        readBuf[readPtr++] = b;
                        if (readPtr == readLen) {
                            handlePacket();
                            readState = ReadState.AWAIT_START;
                        }
                        break;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendPacket(byte id, byte[] data) {
        byte[] output = new byte[data.length + 3];
        output[0] = START_BYTE;
        output[1] = (byte) (data.length + 1);
        output[2] = id;
        System.arraycopy(data, 0, output, 3, data.length);

        try {
            out.write(output);
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void ping() {
        long time = System.currentTimeMillis();

        byte[] payload = new byte[8];
        for (int i = 0; i < 8; i++) {
            int shift = i * 8;
            long mask = 0xFFL << shift;

            payload[i] = (byte) ((time & mask) >> shift);
        }

        sendPacket(PACKET_OUT_PING, payload);
    }

    // Expects at most 32 buttons and 8 axes
    public void sendControllerData(int controller, boolean[] buttons, float[] axes) {
        int buttonMask = 0;
        for (int i = 0; i < buttons.length; i++) {
            buttonMask |= buttons[i] ? 1 << i : 0;
        }

        // 1 byte controller id
        // 4 byte button mask
        // 8 byte axis data
        byte[] data = new byte[1 + 4 + 8];
        data[0] = (byte) controller;
        data[1] = (byte)  (buttonMask & 0x000000FF);
        data[2] = (byte) ((buttonMask & 0x0000FF00) >> 8);
        data[3] = (byte) ((buttonMask & 0x00FF0000) >> 16);
        data[4] = (byte) ((buttonMask & 0xFF000000) >> 24);

        for (int i = 0; i < 8; i++) {
            float axis = i < axes.length ? axes[i] : 0;
            if (axis > 1) axis = 1;
            if (axis < -1) axis = -1;

            data[i + 5] = (byte) (axis * 127);
        }

        sendPacket(PACKET_OUT_CONTROLS, data);
    }

    public void updateState(RobotState state) {
        byte[] data = {
                (byte) (state == RobotState.TELEOP ? 1 : 0)
        };

        sendPacket(PACKET_OUT_STATE, data);
    }

    public void beginWifiUpload() {
        byte[] data = new byte[4];
        data[0] = (byte)  (WIFI_UPLOAD_MAGIC & 0x000000FF);
        data[1] = (byte) ((WIFI_UPLOAD_MAGIC & 0x0000FF00) >> 8);
        data[2] = (byte) ((WIFI_UPLOAD_MAGIC & 0x00FF0000) >> 16);
        data[3] = (byte) ((WIFI_UPLOAD_MAGIC & 0xFF000000) >> 24);

        sendPacket(PACKET_OUT_INIT_WIFI_UPLOAD, data);
    }
}
