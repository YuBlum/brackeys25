#ifndef __MAP_H__
#define __MAP_H__

#define MAP_W 25
#define MAP_H 20

bool map_line_intersects_wall(struct v2 a, struct v2 b);
void map_init(void);
char map_get_tile(struct v2 position);
void map_render(void);

#endif/*__MAP_H__*/
