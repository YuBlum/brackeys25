#ifndef __MAP_H__
#define __MAP_H__

#define MAP_W 25
#define MAP_H 20

bool map_line_intersects_wall(struct v2 a, struct v2 b, struct v2 *out_point);
bool map_indirect_los_to_player(struct entity *e, float dt, struct v2 *out_point);
void map_init(void);
char map_get_tile(struct v2 position);
void map_update(void);

#endif/*__MAP_H__*/
