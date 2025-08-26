void
entity_manager_update(float dt) {
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    e->flags = e->next_flags;
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, KEYBOARD_CONTROLLED) && !entity_has_one_of_flags(e, ATTACKING|KNOCKBACK)) keyboard_control(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, CHECK_TO_FOLLOW) && !entity_has_one_of_flags(e, FOLLOW)) check_to_follow_target(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, FOLLOW) && !entity_has_one_of_flags(e, KNOCKBACK)) follow_target(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, MOVABLE)) get_next_position(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, MOVABLE)) move(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, MOVABLE|WIGGLE) && !entity_has_one_of_flags(e, ATTACKING|KNOCKBACK)) wiggle_animation(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, MOVABLE|RENDER_SPRITE) && !entity_has_one_of_flags(e, ATTACKING|KNOCKBACK)) change_sprite_looking_direction(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, KNOCKBACK)) knockback(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, HAS_WEAPON) && !entity_has_one_of_flags(e, ATTACKING)) update_weapon(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, HAS_WEAPON)) update_weapon_position(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, HAS_WEAPON|ATTACKING)) update_attack(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, DEPTH_BY_BOTTOM)) update_depth_by_bottom(e, dt);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, RENDER_SPRITE)) render_sprite(e);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, RENDER_COLLIDER)) render_collider(e);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, RENDER_HITBOX)) render_hitbox(e);
  }
  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {
    auto e = g_entities.cached[i];
    if (entity_has_flags(e, RENDER_VIEW_RADIUS)) render_view_radius(e);
  }
}
