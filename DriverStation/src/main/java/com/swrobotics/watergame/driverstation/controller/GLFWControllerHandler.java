package com.swrobotics.watergame.driverstation.controller;

import static org.lwjgl.glfw.GLFW.*;

public class GLFWControllerHandler {
    public GLFWControllerHandler(long windowId, ControllerManager manager) {
        for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
            int id = GLFW_JOYSTICK_1 + i;

            if (glfwJoystickPresent(id)) {
                System.out.println("Detected controller " + id);
                manager.addController(new GLFWController(id));
            }
        }
    }
}
