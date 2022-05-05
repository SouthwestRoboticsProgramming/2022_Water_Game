package com.swrobotics.watergame.driverstation;

public class DriverStation {
    public static void main(String[] args) {
        TetheredConnection conn = new TetheredConnection();

        boolean on = false;
        while (true) {
            on = !on;
            conn.sendTest(on);

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
