#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include "libft.h"
# include "libftprintf.h"
# include "libftsscanf.h"
# include "libgc.h"
# include "libgnl.h"


# define WIDTH 500
# define HEIGHT 500

/* Map parsing structures */

typedef struct s_vector
{
	double	x;
	double	y;
}	t_vector;

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
	char	**grid;
	int		width;
	int		height;
	int		*line_lengths;
	int		player_x;
	int		player_y;
	char	player_dir;
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
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
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

#endif
