package com.swrobotics.watergame.driverstation.controller;

import java.util.ArrayList;
import java.util.List;

import static imgui.ImGui.*;

public class ControllerManager {
    private final List<Controller> controllers;
    private int selection;

    public ControllerManager() {
        controllers = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            controllers.add(new NullController());
        }

        selection = 0;
    }

    public void addController(Controller c) {
        controllers.add(c);
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
