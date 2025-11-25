#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include "libft.h"
# include "libftprintf.h"
# include "libftsscanf.h"
# include "libgc.h"
# include "libgnl.h"
# include <limits.h>
# include <sys/time.h>

# define WIDTH 1200
# define HEIGHT 800

/* Field of View (FOV) settings */
# define FOV 60.0
# define FOV_RAD (FOV * 3.14159265359 / 180.0)
# define PLANE_DIST 0.66

/* FPS and timing */
# define TARGET_FPS 60
# define FRAME_TIME_US 16666
# define MOVE_SPEED 3.0
# define ROT_SPEED 2.0

/* Minimap settings */
# define MINIMAP_SCALE 15
# define MINIMAP_OFFSET_X 10
# define MINIMAP_OFFSET_Y 10
# define MINIMAP_VIEWPORT_TILES 15
# define MINIMAP_COLOR_WALL 0xFFFFFF
# define MINIMAP_COLOR_FLOOR 0x333333
# define MINIMAP_COLOR_PLAYER 0xFF0000
# define MINIMAP_COLOR_RAY 0x00FF00
# define MINIMAP_COLOR_BORDER 0x666666

/* Key codes */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

/* Map tile types */
# define WALL '1'
# define WALKABLE '0'
# define NOT_WALKABLE ' '

/* Map parsing structures */

typedef enum e_direction
{
	NORTH,
	SOUTH,
	WEST,
	EAST
}	t_direction;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_textures
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
}	t_textures;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
	int				player_pos[2];
	t_direction		player_dir;
}	t_map;

typedef struct s_config
{
	t_textures	textures;
	t_color		floor;
	t_color		ceiling;
	t_map		map;
	int			config_flags;
}	t_config;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_time
{
	long	last_frame;
	double	delta_time;
}	t_time;

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_image;

typedef struct s_texture
{
	t_image	img;
	int		width;
	int		height;
}	t_texture;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_ray;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_config	config;
	t_player	player;
	t_image		img;
	t_keys		keys;
	t_time		time;
	t_texture	textures[4];
}	t_game;

/* Config flags for validation */
# define FLAG_NO 0x01
# define FLAG_SO 0x02
# define FLAG_WE 0x04
# define FLAG_EA 0x08
# define FLAG_F  0x10
# define FLAG_C  0x20
# define FLAG_ALL 0x3F

/* Exit */
void	ft_exit(int code, const char *message);

/* Parsing functions */
void	print_config(t_config *config);
void	print_map(char **grid, int height);
int		parse_config_file(t_config *config, char *path);
int		parse_config_line(t_config *config, const char *line);

/* Texture parsing functions */
int		parse_texture_no(t_config *config, const char *line);
int		parse_texture_so(t_config *config, const char *line);
int		parse_texture_we(t_config *config, const char *line);
int		parse_texture_ea(t_config *config, const char *line);
int		set_texture(t_config *config, char **texture, char *path, int flag);

/* Color parsing functions */
int		parse_color_floor(t_config *config, const char *line);
int		parse_color_ceiling(t_config *config, const char *line);
int		set_color(t_config *config, t_color *color, int flag);
int		validate_color(int r, int g, int b);

/* Map parsing functions */
int		parse_map(t_config *config, const char *path);
int		read_map_content(t_config *config, const char *path);
int		find_player(t_config *config);
int		validate_map_walls(t_config *config);
int		check_internal_walls(t_config *config);
int		check_sides(t_config *config, int row);

/* Map utility functions */
int		is_map_char(char c);
int		is_map_line(const char *line);
int		get_line_length(const char *line);
int		store_map_line(t_config *config, char *line, int idx);

/* Game/MLX functions */
void	init_mlx(t_game *game);
int		close_window(void *game);
int		handle_keypress(int keycode, void *game);
int		handle_keyrelease(int keycode, void *game);
void	*game_loop(void *game);

/* Timing functions */
long	get_time_us(void);
void	update_delta_time(t_game *game);
void	cap_framerate(t_game *game);

/* Player functions */
void	init_player(t_game *game);
int		can_move_to(t_game *game, double new_x, double new_y);
void	move_forward(t_game *game, double move_speed);
void	move_backward(t_game *game, double move_speed);
void	strafe_left(t_game *game, double move_speed);
void	strafe_right(t_game *game, double move_speed);
void	rotate_left(t_game *game, double rot_speed);
void	rotate_right(t_game *game, double rot_speed);
void	process_movement(t_game *game);

/* Texture functions */
void	load_textures(t_game *game);
int		get_texture_color(t_texture *texture, int x, int y);

/* Raycasting functions */
void		init_ray(t_game *game, t_ray *ray, int x);
void		calculate_step_and_side_dist(t_game *game, t_ray *ray);
void		perform_dda(t_game *game, t_ray *ray);
void		calculate_wall_distance(t_game *game, t_ray *ray);
void		render_3d(t_game *game);
void		draw_wall_stripe(t_game *game, t_ray *ray, int x);
t_texture	*get_wall_texture(t_game *game, t_ray *ray);
int			calculate_tex_x(t_ray *ray, double wall_x, int tex_width);
double		calculate_wall_x(t_game *game, t_ray *ray);

/* Rendering functions */
void	render_frame(t_game *game);
void	render_minimap(t_game *game);
void	draw_minimap_tile(t_game *game, int x, int y, int color);
void	draw_minimap_player(t_game *game);
void	draw_minimap_rays(t_game *game);
void	my_mlx_pixel_put(t_image *img, int x, int y, int color);
int		get_tile_color(t_game *game, int map_x, int map_y);
void	get_camera_start(t_game *game, int *cam_start_x, int *cam_start_y);
void	get_player_center(t_game *game, int *center_x, int *center_y);
int		check_wall_hit(t_game *game, int map_x, int map_y);
int		get_max_steps(int dx, int dy);
void	init_line_vars(int *coords, float *vars);

#endif
