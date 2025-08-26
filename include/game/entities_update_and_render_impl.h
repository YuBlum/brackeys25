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
    if (entity_get_flags(e, KEYBOARD_CONTROLLED) && !entity_get_flags(e, ATTACKING)) keyboard_control(e, dt);
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
    if (entity_get_flags(e, MOVABLE|WIGGLE) && !entity_get_flags(e, ATTACKING)) wiggle_animation(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, MOVABLE|RENDER_SPRITE) && !entity_get_flags(e, ATTACKING)) change_sprite_looking_direction(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, HAS_WEAPON) && !entity_get_flags(e, ATTACKING)) update_weapon(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, HAS_WEAPON)) update_weapon_position(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_get_flags(e, HAS_WEAPON|ATTACKING)) update_attack(e, dt);
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
