package com.swrobotics.watergame.driverstation;

public class DriverStation {
    public static void main(String[] args) throws Exception {
        TetheredConnection conn = new TetheredConnection();

        int i = 0;
        boolean b = false;
        while (true) {
            conn.read();

            Thread.sleep(10);

            i++;
            if (i == 100) {
                i = 0;
                b = !b;

                conn.ping();
                conn.sendControllerData(
                        0,
                        new boolean[] {
                                b, false, b, false,
                                false, false, false, false,
                                false, false, false, false,
                                false, false, false, false,
                                false, false, false, false,
                                false, false, false, false,
                                false, false, false, false,
                                false, false, false, false
                        },
                        new double[] {
                                0, 0, 0, 0,
                                0, 0, 0, 0
                        }
                );
            }
        }
    }
}
