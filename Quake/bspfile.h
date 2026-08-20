/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2010-2014 QuakeSpasm developers

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __BSPFILE_H
#define __BSPFILE_H

// upper design bounds

#define	MAX_MAP_HULLS         (1 <<  2)

#define	MAX_MAP_MODELS        (1 <<  8)
#define	MAX_MAP_BRUSHES       (1 << 12)
#define	MAX_MAP_ENTITIES      (1 << 10)
#define	MAX_MAP_ENTSTRING     (1 << UINT16_WIDTH)

#define	MAX_MAP_PLANES        INT16_MAX
#define	MAX_MAP_NODES         INT16_MAX // because negative shorts are contents
#define	MAX_MAP_CLIPNODES     INT16_MAX
//#define	MAX_MAP_LEAFS		80000 //johnfitz -- was 8192
#define	MAX_MAP_VERTS        UINT16_MAX
#define	MAX_MAP_FACES        UINT16_MAX
#define	MAX_MAP_MARKSURFACES UINT16_MAX
#define	MAX_MAP_TEXINFO      (1 << 12)
#define	MAX_MAP_EDGES        256000
#define	MAX_MAP_SURFEDGES    512000
#define	MAX_MAP_TEXTURES     (1 <<  9)
#define	MAX_MAP_MIPTEX       0x200000
#define	MAX_MAP_LIGHTING     0x100000
#define	MAX_MAP_VISIBILITY   0x100000

#define	MAX_MAP_PORTALS      (1 << UINT16_WIDTH)

// key / value pair sizes

#define	MAX_KEY              (1 <<  5)
#define	MAX_VALUE            (1 << 10)

//=============================================================================


#define BSPVERSION	29

#ifdef BSP29_VALVE
#define BSPVERSION_VALVE 30
#endif

/* RMQ support (2PSB). 32bits instead of shorts for all but bbox sizes (which
 * still use shorts) */
#define BSP2VERSION_2PSB (('B' << 24) | ('S' << 16) | ('P' << 8) | '2')

/* BSP2 support. 32bits instead of shorts for everything (bboxes use floats) */
#define BSP2VERSION_BSP2 (('B' << 0) | ('S' << 8) | ('P' << 16) | ('2'<<24))

// Quake64
#define BSPVERSION_QUAKE64 (('Q' << 24) | ('6' << 16) | ('4' << 8) | ' ')

#define	TOOLVERSION	2

#pragma pack(push,1)
typedef struct
{
	int32_t		fileofs, filelen;
} lump_t;
#pragma pack(pop)

#define	LUMP_ENTITIES      0
#define	LUMP_PLANES        1
#define	LUMP_TEXTURES      2
#define	LUMP_VERTEXES	   3
#define	LUMP_VISIBILITY    4
#define	LUMP_NODES         5
#define	LUMP_TEXINFO       6
#define	LUMP_FACES         7
#define	LUMP_LIGHTING      8
#define	LUMP_CLIPNODES	   9
#define	LUMP_LEAFS        10
#define	LUMP_MARKSURFACES 11
#define	LUMP_EDGES        12
#define	LUMP_SURFEDGES    13
#define	LUMP_MODELS       14

#define	HEADER_LUMPS      15

#pragma pack(push,1)
typedef struct
{
	float		mins[3];
	float		maxs[3];
	float		origin[3];
	int32_t		headnode[MAX_MAP_HULLS];
	int32_t		visleafs;		// not including the solid leaf 0
	int32_t		firstface;
	int32_t		numfaces;
} dmodel_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t			version;
	lump_t		lumps[HEADER_LUMPS];
} dheader_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t			nummiptex;
	int32_t			dataofs[4];		// [nummiptex]
} dmiptexlump_t;
#pragma pack(pop)

#define	MIPLEVELS	4
#pragma pack(push,1)
typedef struct miptex_s
{
	char		name[16];
	uint32_t	width, height;
	uint32_t	offsets[MIPLEVELS];		// four mip maps stored
} miptex_t;
#pragma pack(pop)
// Quake64
#pragma pack(push,1)
typedef struct miptex64_s
{
	char		name[16];
	uint32_t	width, height;
	uint32_t	shift;
	uint32_t	offsets[MIPLEVELS];		// four mip maps stored
} miptex64_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	float	point[3];
} dvertex_t;
#pragma pack(pop)

// 0-2 are axial planes
#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2

// 3-5 are non-axial planes snapped to the nearest
#define	PLANE_ANYX		3
#define	PLANE_ANYY		4
#define	PLANE_ANYZ		5

#pragma pack(push,1)
typedef struct
{
	float	normal[3];
	float	dist;
	int		type;		// PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
} dplane_t;
#pragma pack(pop)

#define	CONTENTS_EMPTY		-1
#define	CONTENTS_SOLID		-2
#define	CONTENTS_WATER		-3
#define	CONTENTS_SLIME		-4
#define	CONTENTS_LAVA		-5
#define	CONTENTS_SKY		-6
#define	CONTENTS_ORIGIN		-7		// removed at csg time
#define	CONTENTS_CLIP		-8		// changed to contents_solid

#define	CONTENTS_CURRENT_0		-9
#define	CONTENTS_CURRENT_90		-10
#define	CONTENTS_CURRENT_180	-11
#define	CONTENTS_CURRENT_270	-12
#define	CONTENTS_CURRENT_UP		-13
#define	CONTENTS_CURRENT_DOWN	-14


// !!! if this is changed, it must be changed in asm_i386.h too !!!
#pragma pack(push,1)
typedef struct
{
	int32_t		planenum;
	int16_t		children[2];	// negative numbers are -(leafs+1), not nodes
	int16_t		mins[3];		// for sphere culling
	int16_t		maxs[3];
	uint16_t	firstface;
	uint16_t	numfaces;	// counting both sides
} dsnode_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t		planenum;
	int32_t		children[2];	// negative numbers are -(leafs+1), not nodes
	int16_t		mins[3];		// for sphere culling
	int16_t		maxs[3];
	uint32_t	firstface;
	uint32_t	numfaces;	// counting both sides
} dl1node_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t		planenum;
	int32_t		children[2];	// negative numbers are -(leafs+1), not nodes
	float		mins[3];		// for sphere culling
	float		maxs[3];
	uint32_t	firstface;
	uint32_t	numfaces;	// counting both sides
} dl2node_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t		planenum;
	int16_t		children[2];	// negative numbers are contents
} dsclipnode_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t planenum;
	int32_t children[2];	// negative numbers are contents
} dlclipnode_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct texinfo_s
{
	float   vecs[2][4];		// [s/t][xyz offset]
	int32_t miptex;
	int32_t flags;
} texinfo_t;
#pragma pack(pop)

#define	TEX_SPECIAL		1		// sky or slime, no lightmap or 256 subdivision
#define TEX_MISSING		2		// johnfitz -- this texinfo does not have a texture

// note that edge 0 is never used, because negative edge nums are used for
// counterclockwise use of the edge in a face
#pragma pack(push,1)
typedef struct
{
	uint16_t v[2];		// vertex numbers
} dsedge_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	uint32_t v[2];		// vertex numbers
} dledge_t;
#pragma pack(pop)

#define	MAXLIGHTMAPS	4
#pragma pack(push,1)
typedef struct
{
	int16_t planenum;
	int16_t side;

	int32_t firstedge;		// we must support > 64k edges
	int16_t	numedges;
	int16_t texinfo;

// lighting info
	byte    styles[MAXLIGHTMAPS];
	int32_t lightofs;		// start of [numstyles*surfsize] samples
} dsface_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	int32_t planenum;
	int32_t side;

	int32_t firstedge;		// we must support > 64k edges
	int32_t numedges;
	int32_t texinfo;

// lighting info
	byte    styles[MAXLIGHTMAPS];
	int32_t lightofs;		// start of [numstyles*surfsize] samples
} dlface_t;
#pragma pack(pop)

#define	AMBIENT_WATER	0
#define	AMBIENT_SKY		1
#define	AMBIENT_SLIME	2
#define	AMBIENT_LAVA	3

#define	NUM_AMBIENTS			4		// automatic ambient sounds

// leaf 0 is the generic CONTENTS_SOLID leaf, used for all solid areas
// all other leafs need visibility info
#pragma pack(push,1)
typedef struct
{
	 int32_t contents;
	 int32_t visofs;				// -1 = no visibility info

	 int16_t mins[3];			// for frustum culling
	 int16_t maxs[3];

	uint16_t firstmarksurface;
	uint16_t nummarksurfaces;

	    byte ambient_level[NUM_AMBIENTS];
} dsleaf_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	 int32_t contents;
	 int32_t visofs;				// -1 = no visibility info

	 int16_t mins[3];			// for frustum culling
	 int16_t maxs[3];

	uint32_t firstmarksurface;
	uint32_t nummarksurfaces;

	    byte ambient_level[NUM_AMBIENTS];
} dl1leaf_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	 int32_t contents;
	 int32_t visofs;				// -1 = no visibility info

	   float mins[3];			// for frustum culling
	   float maxs[3];

	uint32_t firstmarksurface;
	uint32_t nummarksurfaces;

  	    byte ambient_level[NUM_AMBIENTS];
} dl2leaf_t;
#pragma pack(pop)

//============================================================================

#ifndef QUAKE_GAME

#define	ANGLE_UP	-1
#define	ANGLE_DOWN	-2


// the utilities get to be lazy and just use large static arrays

extern	int32_t  nummodels;
extern	dmodel_t dmodels[MAX_MAP_MODELS];

extern	int32_t  visdatasize;
extern	byte     dvisdata[MAX_MAP_VISIBILITY];

extern	int32_t  lightdatasize;
extern	byte     dlightdata[MAX_MAP_LIGHTING];

extern	int32_t  texdatasize;
extern	byte     dtexdata[MAX_MAP_MIPTEX]; // (dmiptexlump_t)

extern	int32_t  entdatasize;
extern	char     dentdata[MAX_MAP_ENTSTRING];

//extern	int			numleafs;
//extern	dleaf_t		dleafs[MAX_MAP_LEAFS];

extern	int32_t     numplanes;
extern	dplane_t    dplanes[MAX_MAP_PLANES];

extern	int32_t     numvertexes;
extern	dvertex_t   dvertexes[MAX_MAP_VERTS];

extern	int32_t     numnodes;
extern	dnode_t     dnodes[MAX_MAP_NODES];

extern	int32_t     numtexinfo;
extern	texinfo_t   texinfo[MAX_MAP_TEXINFO];

extern	int32_t     numfaces;
extern	dface_t	    dfaces[MAX_MAP_FACES];

extern	int32_t     numclipnodes;
extern	dclipnode_t dclipnodes[MAX_MAP_CLIPNODES];

extern	int32_t	    numedges;
extern	dedge_t	    dedges[MAX_MAP_EDGES];

extern	 int32_t    nummarksurfaces;
extern	uint16_t    dmarksurfaces[MAX_MAP_MARKSURFACES];

extern	int32_t     numsurfedges;
extern	int32_t     dsurfedges[MAX_MAP_SURFEDGES];


void DecompressVis (byte *in, byte *decompressed);
int CompressVis (byte *vis, byte *dest);

void	LoadBSPFile (char *filename);
void	WriteBSPFile (char *filename);
void	PrintBSPFileSizes (void);

//===============

#pragma pack(push,1)
typedef struct epair_s
{
	struct epair_s	*next;
	char	*key;
	char	*value;
} epair_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
	vec3_t   origin;
	int32_t  firstbrush;
	int32_t  numbrushes;
	epair_t	*epairs;
} entity_t;
#pragma pack(pop)

extern	int32_t  num_entities;
extern	entity_t entities[MAX_MAP_ENTITIES];

void	ParseEntities (void);
void	UnparseEntities (void);

void 	SetKeyValue (entity_t *ent, char *key, char *value);
char 	*ValueForKey (entity_t *ent, char *key);
// will return "" if not present

vec_t	FloatForKey (entity_t *ent, char *key);
void 	GetVectorForKey (entity_t *ent, char *key, vec3_t vec);

epair_t *ParseEpair (void);

#endif	/* QUAKE_GAME */

#endif	/* __BSPFILE_H */
