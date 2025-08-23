#include <string.h>
#include <GLFW/glfw3.h>
#include "engine/core.h"
#include "engine/math.h"
#include "engine/window.h"
#include "engine/renderer.h"
#if WASM
#  include <emscripten.h>
#endif

struct window {
  GLFWwindow *handle;
  struct v2 cursor;
  float time;
  bool keys[KEY_AMOUNT];
  bool pkeys[KEY_AMOUNT];
  bool btns[BUTTON_AMOUNT];
  bool pbtns[BUTTON_AMOUNT];
};

static struct window g_window;

static void
key_callback(GLFWwindow* _window, int key, int _scancode, int action, int _mods) {
  (void)_window; (void)_scancode; (void) _mods;
  if (action != GLFW_PRESS && action != GLFW_RELEASE) return;
  switch (key) {
    case GLFW_KEY_ESCAPE: g_window.keys[KEY_EXIT]      = action == GLFW_PRESS; break;
    case GLFW_KEY_D:      g_window.keys[KEY_RIGHT]     = action == GLFW_PRESS; break;
    case GLFW_KEY_A:      g_window.keys[KEY_LEFT]      = action == GLFW_PRESS; break;
    case GLFW_KEY_W:      g_window.keys[KEY_UP]        = action == GLFW_PRESS; break;
    case GLFW_KEY_S:      g_window.keys[KEY_DOWN]      = action == GLFW_PRESS; break;
    case GLFW_KEY_SPACE:  g_window.keys[KEY_INTERACT]  = action == GLFW_PRESS; break;
    case GLFW_KEY_F1:     g_window.keys[KEY_DEBUG0]    = action == GLFW_PRESS; break;
  }
}

static void
button_callback(GLFWwindow* _window, int button, int action, int _mods) {
  (void)_window; (void) _mods;
  if (action != GLFW_PRESS && action != GLFW_RELEASE) return;
  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:  g_window.btns[BTN_LEFT]  = action == GLFW_PRESS; break;
    case GLFW_MOUSE_BUTTON_RIGHT: g_window.btns[KEY_RIGHT] = action == GLFW_PRESS; break;
  }
}

static void
cursor_position_callback(GLFWwindow* _window, double xpos, double ypos) {
  (void)_window;
  g_window.cursor.x = xpos/(GAME_S*UNIT_PER_PIXEL)-GAME_W*0.5f;
  g_window.cursor.y = GAME_H*0.5f-ypos/(GAME_S*UNIT_PER_PIXEL);
}

bool
window_make(void) {
  log_infol("making window...");
  if (!glfwInit()) {
    log_errorl("couldn't initialize glfw");
    return false;
  }
  log_infol("glfw initialized");
  glfwWindowHint(GLFW_RESIZABLE, false);
#if WASM
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  g_window.handle = glfwCreateWindow(WINDOW_W, WINDOW_H, WINDOW_TITLE, 0, 0);
  if (!g_window.handle) {
    log_errorl("couldn't make window handle");
    return false;
  }
  glfwMakeContextCurrent(g_window.handle);
  //glfwSwapInterval(0);
  log_infolf("window created with %d %d dimensions", WINDOW_W, WINDOW_H);
  const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if (vidmode) {
    glfwSetWindowPos(g_window.handle, vidmode->width / 2 - WINDOW_W / 2, vidmode->height / 2 - WINDOW_H / 2);
    log_infol("window centered");
  } else {
    log_warnl("couldn't center window");
  }
  (void)memset(g_window.keys, false, sizeof (bool) * KEY_AMOUNT);
  (void)glfwSetKeyCallback(g_window.handle, key_callback);
  (void)glfwSetCursorPosCallback(g_window.handle, cursor_position_callback);
  (void)glfwSetMouseButtonCallback(g_window.handle, button_callback);
  glfwSetInputMode(g_window.handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(g_window.handle, WINDOW_W / 2, WINDOW_H / 2);
  log_infol("window input setup");
  log_infol("window creation complete!");
  return true;
}

void
window_destroy(void) {
  glfwTerminate();
}

void
window_close(void) {
  (void)glfwSetWindowShouldClose(g_window.handle, true);
}

#if WASM
static void
window_main_loop(struct arena *arena) {
  if (!game_loop(arena)) emscripten_cancel_main_loop();
}
#endif

void
window_run(struct arena *arena) {
#if WASM
  emscripten_set_main_loop_arg((em_arg_callback_func)window_main_loop, arena, 0, true);
#endif
#ifndef WASM
  while (game_loop(arena));
#endif
}

float 
window_frame_begin(void) {
  float prv_time = g_window.time;
  g_window.time = glfwGetTime();
  glfwPollEvents();
  glfwSwapBuffers(g_window.handle);
  return g_window.time - prv_time;
}

bool
window_frame_end(void) {
  memcpy(g_window.pkeys, g_window.keys, sizeof (bool) * KEY_AMOUNT);
  memcpy(g_window.pbtns, g_window.btns, sizeof (bool) * BUTTON_AMOUNT);
  return !glfwWindowShouldClose(g_window.handle);
}

bool
window_is_key_press(enum key key) {
#if DEV
  if (key >= KEY_AMOUNT) {
    log_errorlf("%s: passing invalid key", __func__);
    return false;
  }
#endif
  return !g_window.pkeys[key] && g_window.keys[key];
}

bool
window_is_key_down(enum key key) {
#if DEV
  if (key >= KEY_AMOUNT) {
    log_errorlf("%s: passing invalid key", __func__);
    return false;
  }
#endif
  return g_window.keys[key];
}

bool
window_is_key_up(enum key key) {
#if DEV
  if (key >= KEY_AMOUNT) {
    log_errorlf("%s: passing invalid key", __func__);
    return false;
  }
#endif
  return !g_window.keys[key];
}

bool
window_is_key_release(enum key key) {
#if DEV
  if (key >= KEY_AMOUNT) {
    log_errorlf("%s: passing invalid key", __func__);
    return false;
  }
#endif
  return g_window.pkeys[key] && !g_window.keys[key];
}

bool
window_is_button_press(enum button btn) {
#if DEV
  if (btn >= BUTTON_AMOUNT) {
    log_errorlf("%s: passing invalid button", __func__);
    return false;
  }
#endif
  return !g_window.pbtns[btn] && g_window.btns[btn];
}

bool
window_is_button_down(enum button btn) {
#if DEV
  if (btn >= BUTTON_AMOUNT) {
    log_errorlf("%s: passing invalid button", __func__);
    return false;
  }
#endif
  return g_window.btns[btn];
}

bool
window_is_button_up(enum button btn) {
#if DEV
  if (btn >= BUTTON_AMOUNT) {
    log_errorlf("%s: passing invalid button", __func__);
    return false;
  }
#endif
  return !g_window.btns[btn];
}

bool
window_is_button_release(enum button btn) {
#if DEV
  if (btn >= BUTTON_AMOUNT) {
    log_errorlf("%s: passing invalid button", __func__);
    return false;
  }
#endif
  return g_window.pbtns[btn] && !g_window.btns[btn];
}

struct v2
window_get_cursor_position(void) {
  return v2_add(g_window.cursor, renderer_get_offset());
}

