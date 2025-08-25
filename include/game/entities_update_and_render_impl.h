void
entity_manager_update(float dt) {
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    e->flags = e->next_flags;
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, FOLLOW_CURSOR)) follow_cursor(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, KEYBOARD_CONTROLLED)) keyboard_control(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, MOVABLE)) get_next_position(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, MOVABLE)) move(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, FOLLOW_CURSOR|STATE_MACHINE)) update_cursor_state(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, RENDER_ANIMATION|LOOPABLE)) loop_animation(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, RENDER_ANIMATION)) update_animation(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, RENDER_ANIMATION)) render_animation(e);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, RENDER_SPRITE)) render_sprite(e);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, RENDER_COLLIDER)) render_collider(e);
  }
}
