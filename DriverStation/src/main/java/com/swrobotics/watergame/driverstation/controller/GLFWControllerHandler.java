package com.swrobotics.watergame.driverstation.controller;

import org.lwjgl.glfw.GLFWJoystickCallback;

import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.glfw.GLFW.*;

public class GLFWControllerHandler {
    private final ControllerManager manager;
    private final GLFWJoystickCallback prevCallback;
    private final Map<Integer, GLFWController> activeControllers;

    public GLFWControllerHandler(long windowId, ControllerManager manager) {
        this.manager = manager;
        activeControllers = new HashMap<>();

        for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
            int id = GLFW_JOYSTICK_1 + i;

            if (glfwJoystickPresent(id)) {
                System.out.println("Detected controller " + id);
                GLFWController controller = new GLFWController(id);
                activeControllers.put(id, controller);
                manager.addController(controller);
            }
        }

        prevCallback = glfwSetJoystickCallback(this::joystickCallback);
    }

    private void joystickCallback(int id, int event) {
        if (prevCallback != null)
            prevCallback.invoke(id, event);

        if (event == GLFW_CONNECTED) {
            GLFWController controller = new GLFWController(id);
            activeControllers.put(id, controller);
            manager.addController(controller);

            System.out.println("Controller " + id + " connected");
        } else if (event == GLFW_DISCONNECTED) {
            if (activeControllers.containsKey(id)) {
                GLFWController controller = activeControllers.get(id);
                manager.removeController(controller);

                System.out.println("Controller " + id + " disconnected");
            }
        }
    }
}
