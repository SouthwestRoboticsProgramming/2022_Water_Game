package com.swrobotics.watergame.driverstation.controller;

import imgui.ImGui;

import java.util.UUID;

public final class NullController implements Controller {
    private final UUID uuid;

    public NullController() {
        uuid = UUID.randomUUID();
    }

    @Override
    public UUID getUUID() {
        return uuid;
    }

    @Override
    public String getDescription() {
        return "---";
    }

    @Override
    public boolean needsUpdateDataToRobot() {
        return false;
    }

    @Override
    public int getAxisCount() {
        return 0;
    }

    @Override
    public float getAxis(int index) {
        return 0;
    }

    @Override
    public int getButtonCount() {
        return 0;
    }

    @Override
    public boolean getButton(int index) {
        return false;
    }

    @Override
    public void renderGuiContent() {
        ImGui.text("No device");
    }
}
