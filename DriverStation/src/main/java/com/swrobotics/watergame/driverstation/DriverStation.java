package com.swrobotics.watergame.driverstation;

import com.swrobotics.watergame.driverstation.controller.ControllerManager;
import com.swrobotics.watergame.driverstation.controller.KeyboardController;
import imgui.app.Application;
import imgui.app.Configuration;

import static imgui.ImGui.*;

public class DriverStation extends Application {
    private ControllerManager controllerManager;

    public DriverStation() {
        controllerManager = new ControllerManager();
    }

    @Override
    public void configure(Configuration config) {
        config.setTitle("Water Game Driver Station");
    }

    @Override
    public void initWindow(Configuration config) {
        super.initWindow(config);
        controllerManager.addController(new KeyboardController(getHandle()));
    }

    @Override
    protected void initImGui(Configuration config) {
        super.initImGui(config);

        styleColorsLight();
    }

    @Override
    public void process() {
        showDemoWindow();

        controllerManager.showControllersWindow();
    }

    public static void main(String[] args) {
        launch(new DriverStation());
    }
}
