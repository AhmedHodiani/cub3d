#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include "libft.h"
# include "libftprintf.h"
# include "libftsscanf.h"
# include "libgc.h"
# include "libgnl.h"
# include <limits.h>


# define WIDTH 500
# define HEIGHT 500

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
	// idk
}	t_player;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_config	config;
	t_player	player;
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
int		parse_config_file(t_config *config, const char *path);
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

#endif
