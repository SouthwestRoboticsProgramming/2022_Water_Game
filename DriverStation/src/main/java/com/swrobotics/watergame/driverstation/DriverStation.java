package com.swrobotics.watergame.driverstation;

import com.swrobotics.watergame.driverstation.controller.ControllerManager;
import com.swrobotics.watergame.driverstation.controller.GLFWControllerHandler;
import com.swrobotics.watergame.driverstation.controller.KeyboardController;
import imgui.app.Application;
import imgui.app.Configuration;

import static imgui.ImGui.*;

public class DriverStation extends Application {
    private ControllerManager controllerManager;
    private TetheredConnection conn;

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
        new GLFWControllerHandler(getHandle(), controllerManager);

        conn = new TetheredConnection();
    }

    @Override
    protected void initImGui(Configuration config) {
        super.initImGui(config);

        Styles.applyDark();
    }

    @Override
    public void process() {
        conn.read();

        controllerManager.updateControllers();
        controllerManager.updateControllerData(conn);

        showDemoWindow();
        controllerManager.showControllersWindow();

        if (begin("test")) {
            if (button("Ping"))
                conn.ping();
        }
        end();

        try {
            Thread.sleep(1000 / 50);
        } catch (InterruptedException e) {
            // Ignore
        }
    }

    public static void main(String[] args) {
        launch(new DriverStation());
    }
}
