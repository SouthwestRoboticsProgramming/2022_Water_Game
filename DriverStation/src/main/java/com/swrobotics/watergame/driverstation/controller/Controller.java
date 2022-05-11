package com.swrobotics.watergame.driverstation.controller;

import java.util.UUID;

public interface Controller {
    UUID getUUID();
    String getDescription();

    boolean needsUpdateDataToRobot();
    void update();

    int getAxisCount();
    float getAxis(int index);

    int getButtonCount();
    boolean getButton(int index);

    void renderGuiContent();
}
