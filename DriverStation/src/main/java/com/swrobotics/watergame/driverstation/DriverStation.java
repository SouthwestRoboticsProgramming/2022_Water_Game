package com.swrobotics.watergame.driverstation;

import com.swrobotics.watergame.driverstation.controller.ControllerManager;
import com.swrobotics.watergame.driverstation.controller.GLFWControllerHandler;
import com.swrobotics.watergame.driverstation.controller.KeyboardController;
import imgui.app.Application;
import imgui.app.Configuration;

import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Properties;

import static imgui.ImGui.*;

public class DriverStation extends Application {
    private static final String PREFERENCES_FILE = "prefs.properties";

    private ControllerManager controllerManager;
    private TetheredConnection conn;
    private OperationWindow operationWindow;

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

        new GLFWControllerHandler(getHandle(), controllerManager);
        controllerManager.addController(new KeyboardController(getHandle()));

        conn = new TetheredConnection();

        operationWindow = new OperationWindow(conn);

        try {
            Properties props = new Properties();
            props.load(new FileReader(PREFERENCES_FILE));

            controllerManager.loadPreferences(props);
        } catch (IOException e) {
            e.printStackTrace();
        }
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
        operationWindow.showGui();

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

    @Override
    protected void disposeImGui() {
        Properties props = new Properties();
        controllerManager.savePreferences(props);

        try {
            props.store(new FileWriter(PREFERENCES_FILE), "Water game driver station preferences");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        launch(new DriverStation());
    }
}
