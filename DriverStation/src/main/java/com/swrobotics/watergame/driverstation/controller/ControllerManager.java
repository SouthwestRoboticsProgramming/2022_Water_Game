package com.swrobotics.watergame.driverstation.controller;

import com.swrobotics.watergame.driverstation.TetheredConnection;

import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import static imgui.ImGui.*;

public class ControllerManager {
    private static final int COUNT = 4;

    private final List<Controller> controllers;
    private int selection;

    public ControllerManager() {
        controllers = new ArrayList<>();
        for (int i = 0; i < COUNT; i++) {
            controllers.add(new NullController());
        }

        selection = 0;
    }

    public void addController(Controller c) {
        for (int i = 0; i < COUNT; i++) {
            if (controllers.get(i) instanceof NullController) {
                controllers.set(i, c);
                return;
            }
        }

        System.out.println("Warning: Too many input devices!");
    }

    public void removeController(Controller c) {
        int index = controllers.indexOf(c);
        if (index >= 0) {
            controllers.set(index, new NullController());
        }
    }

    public void updateControllers() {
        for (Controller c : controllers) {
            c.update();
        }
    }

    public void updateControllerData(TetheredConnection conn) {
        for (int i = 0; i < controllers.size(); i++) {
            Controller c = controllers.get(i);

            if (c.needsUpdateDataToRobot()) {
                boolean[] buttons = new boolean[c.getButtonCount()];
                float[] axes = new float[c.getAxisCount()];

                for (int j = 0; j < buttons.length; j++)
                    buttons[j] = c.getButton(j);
                for (int j = 0; j < axes.length; j++)
                    axes[j] = c.getAxis(j);

                conn.sendControllerData(i, buttons, axes);
            }
        }
    }

    public void loadPreferences(Properties props) {
        for (Controller c : controllers) {
            c.loadPreferences(props);
        }
    }

    public void savePreferences(Properties props) {
        for (Controller c : controllers) {
            c.savePreferences(props);
        }
    }

    public void showControllersWindow() {
        if (begin("Input Devices")) {
            for (int n = 0; n < controllers.size(); n++) {
                Controller controller = controllers.get(n);

                text(String.valueOf(n));
                sameLine();
                selectable(controller.getDescription() + "##" + controller.getUUID(), n == selection);
                if (isItemClicked(0)) {
                    selection = n;
                }

                if (isItemActive() && !isItemHovered())
                {
                    int n_next = n + (getMouseDragDelta(0).y < 0.f ? -1 : 1);
                    if (n_next >= 0 && n_next < controllers.size())
                    {
                        controllers.set(n, controllers.get(n_next));
                        controllers.set(n_next, controller);
                        resetMouseDragDelta();

                        if (selection == n)
                            selection = n_next;
                    }
                }
            }

            separator();

            Controller selected = controllers.get(selection);
            selected.renderGuiContent();
        }
        end();
    }
}
